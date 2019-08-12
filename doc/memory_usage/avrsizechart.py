import sys
import os
import re
import subprocess
import argparse
import matplotlib.pyplot as plt

def main():
	parser = argparse.ArgumentParser(description="Creates a pie chart with data collected from the \
		avr-size CLI tool and presents it using matplotlib.")
	parser.add_argument("--total-size", action="store_true",
		help="display how much total memory has been used up.")
	args = parser.parse_args()

	avr_cmd = "avr-size " + get_object_path() + "*.o"
	avrsize_output = subprocess.check_output(avr_cmd).decode(sys.stdout.encoding).strip()
	avr_row_dicts = get_row_dicts_from_avrsize_output(avrsize_output)

	if args.total_size:
		create_total_size_plot(avr_row_dicts)
	else:
		create_avrsize_plot(avr_row_dicts)

	plt.tight_layout(pad=4)
	plt.show()


def get_object_path():
	""" Returns the path to the object files created from compilation. """
	git_cmd = ["git", "rev-parse", "--show-toplevel"]
	root_path = subprocess.check_output(git_cmd).decode(sys.stdout.encoding).strip()
	root_path = "C:/" + root_path.lstrip("/cygdrive/")
	return root_path + "/build/obj/bin/"


def get_row_dicts_from_avrsize_output(avrsize_output):
	"""
	Takes the output from the avr-size command and marshals it into a list of dicts.
	Dict has the keys {"text", "data", "bss", "dec", "hex", filename"}.
	"""
	dicts = []
	for row in avrsize_output.splitlines()[1:]:
		regex_pattern = re.compile("[\t]+")
		rows = re.split(regex_pattern, row)
		row = row.replace(' ', '').split('\t')
		row_dict = {"text" : int(row[0]), "data" : int(row[1]), "bss" : int(row[2]),
			"dec" : int(row[3]), "hex" : int(row[4], 16), "filename" : os.path.split(row[5])[1]}
		dicts.append(row_dict)

	return dicts


def create_avrsize_plot(avr_row_dicts):
	"""
	Takes a list of dicts containing program size information and creates a matplotlib pie chart
	that presents how big each object file is.
	"""
	labels = [row_dict["filename"] for row_dict in avr_row_dicts]
	sizes  = [row_dict["dec"] for row_dict in avr_row_dicts]

	fig, ax = plt.subplots()
	fig.subplots_adjust(wspace=1.1, right=0.9)
	fig.canvas.set_window_title("Output of avr-size")
	wedges = ax.pie(sizes, labels=labels, autopct='%1.1f%%', shadow=True, startangle=0)
	ax.axis('equal')
	ax.set_title("Memory usage")


def create_total_size_plot(avr_row_dicts):
	"""
	Takes a list of dicts containing program size information and creates a matplotlib pie chart
	that presents how much of the total program size has been used up.
	"""
	atmega328_prog_memory_size = int(32e3) # 32KB
	labels = ["used memory", "free memory"]
	used_size  = sum([row_dict["dec"] for row_dict in avr_row_dicts])
	unused_size = atmega328_prog_memory_size - used_size
	sizes = [used_size, unused_size]

	fig, ax = plt.subplots()
	fig.subplots_adjust(wspace=1.1, right=0.9)
	fig.canvas.set_window_title("Output of avr-size")
	wedges = ax.pie(sizes, labels=labels, autopct='%1.1f%%', shadow=True, startangle=0)
	ax.axis('equal')
	ax.set_title("Memory usage")

if __name__ == "__main__":
	main()