import sys
import os
import subprocess
import argparse
import matplotlib.pyplot as plt


def main():
	parser = create_parser()
	args = parser.parse_args()

	avrsize_output = get_avrsize_output()
	avr_row_dicts = get_row_dicts_from_avrsize_output(avrsize_output)

	if args.print:
		print(avrsize_output)
		if args.show_bootloader:
			print("    512       0       0     512     200 atmega328 bootloader", end="")
		return

	if args.total_size:
		create_total_size_plot(avr_row_dicts, args.show_bootloader)
	else:
		create_file_size_plot(avr_row_dicts, args.show_bootloader)

	plt.tight_layout(pad=4)

	if args.output:
		plt.savefig(args.output[0])
	else:
		plt.show()


def create_parser():
	""" Create a CLI front-end and return it. """
	parser = argparse.ArgumentParser(description="Creates a pie chart with data collected from \
		the avr-size CLI tool and presents it using matplotlib.")
	total_size_msg = "Display how much total memory has been used up."
	print_msg = "Print output from avr-size to terminal instead of creating pie chart"
	show_bootloader_msg = "Add bootloader size to memory usage. If option not used, maximum \
		memory will be set to 32KB minus the bootloader size for the total size of available \
		program memory."
	parser.add_argument("-t","--total-size", action="store_true", help=total_size_msg)
	parser.add_argument("-p", "--print", action="store_true", help=print_msg)
	parser.add_argument("-b", "--show-bootloader", action="store_true", help=show_bootloader_msg)
	parser.add_argument("-o", "--output", nargs=1)
	return parser


def get_avrsize_output():
	""" Executes the avr-size command and returns the result as a string. """
	avr_cmd = "avr-size " + get_object_path() + "*.o"
	return subprocess.check_output(avr_cmd).decode(sys.stdout.encoding).strip()


def get_object_path():
	""" Return the path to the object files created from compilation. """
	git_cmd = ["git", "rev-parse", "--show-toplevel"]
	root_path = subprocess.check_output(git_cmd).decode(sys.stdout.encoding).strip()
	root_path = "C:/" + root_path.lstrip("/cygdrive/")
	return root_path + "/build/obj/bin/"


def get_row_dicts_from_avrsize_output(avrsize_output):
	"""
	Take the output from the avr-size command and marshals it into a list of dicts.
	Dict has the keys {"text", "data", "bss", "dec", "hex", filename"}.
	"""
	dicts = []
	for row in avrsize_output.splitlines()[1:]:
		row = row.replace(' ', '').split('\t')
		row_dict = {"text" : int(row[0]), "data" : int(row[1]), "bss" : int(row[2]),
			"dec" : int(row[3]), "hex" : int(row[4], 16), "filename" : os.path.split(row[5])[1]}
		dicts.append(row_dict)

	return dicts


def create_file_size_plot(avr_row_dicts, show_bootloader):
	"""
	Take a list of dicts containing program size information and creates a matplotlib pie chart
	that presents how big each object file is.

	If show_bootloader is True, then the bootloader will be shown as a pie slice.
	"""
	labels = [row_dict["filename"].rstrip(".o") for row_dict in avr_row_dicts]
	sizes  = [row_dict["dec"] for row_dict in avr_row_dicts]

	# Add bootloader stats
	if show_bootloader:
		labels.append("bootloader")
		sizes.append(512)

	# Create plot
	fig, ax = plt.subplots()
	fig.subplots_adjust(wspace=1.1, right=0.9)
	fig.canvas.set_window_title("Output of avr-size")
	wedges = ax.pie(sizes, labels=labels, autopct='%1.1f%%', shadow=True, startangle=0)
	ax.axis('equal')
	ax.set_title("Memory usage")


def create_total_size_plot(avr_row_dicts, show_bootloader):
	"""
	Take a list of dicts containing program size information and creates a matplotlib pie chart
	that presents how much of the total program size has been used up.

	If show_bootloader is True, then the bootloader will be counted into the used size. If it is
	False, then instead the total available program memory will exclude the bootloader.
	"""
	bootloader_size = 512
	atmega328_prog_memory_size = int(32*1024) # 32KB
	labels = ["used memory", "free memory"]
	used_size  = sum([row_dict["dec"] for row_dict in avr_row_dicts])
	unused_size = atmega328_prog_memory_size - used_size

	if show_bootloader:
		used_size += bootloader_size
	else:
		unused_size -= bootloader_size

	sizes = [used_size, unused_size]

	fig, ax = plt.subplots()
	fig.subplots_adjust(wspace=1.1, right=0.9)
	fig.canvas.set_window_title("Output of avr-size")
	wedges = ax.pie(sizes, labels=labels, autopct='%1.1f%%', shadow=True, startangle=0)
	ax.axis('equal')
	ax.set_title("Memory usage")


if __name__ == "__main__":
	main()
