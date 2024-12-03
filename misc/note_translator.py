import re

def parse_note(note):
    """Convert a single note to the desired output format."""
    if note.startswith("NOTE_"):
        match = re.match(r"NOTE_([A-GS]+)(\d),(-?\d+)", note)
        if match:
            name, octave, duration = match.groups()
            # Adjust note name for the required output format
            if name.endswith("S"):  # Sharp note
                name = name.replace("S", "s")
            else:  # Natural note
                name += "n"
            # Convert duration
            duration = int(duration)
            if duration < 0:  # Dotted note
                duration = int(32 / abs(duration) * 1.5)
            else:
                duration = int(32 / duration)
            return f"N({name}, {octave}, {duration})"
    elif note.startswith("REST"):
        match = re.match(r"REST,(-?\d+)", note)
        if match:
            duration = int(match.group(1))
            if duration < 0:  # Dotted rest
                duration = int(32 / abs(duration) * 1.5)
            else:
                duration = int(32 / duration)
            return f"PAUSE({duration})"
    return None

def process_line(line):
    """Process a single line of input and handle comments."""
    result = []
    comment = ""
    # Separate comments if present
    if "//" in line:
        line, comment = line.split("//", 1)
    # Split notes properly while keeping the structure intact
    notes = re.findall(r"(NOTE_[A-GS]+\d,-?\d+|REST,-?\d+)", line)
    for note in notes:
        translated_note = parse_note(note)
        if translated_note:
            result.append(translated_note)
    return ", ".join(result) + (", // " + comment.strip() if comment else ",")

def translate_file(input_file, output_file):
    """Translate the input file to the output format."""
    with open(input_file, "r") as infile, open(output_file, "w") as outfile:
        for line in infile:
            line = line.strip()
            if line:  # Skip empty lines
                translated_line = process_line(line)
                outfile.write(translated_line + "\n")
            else:
                outfile.write("\n")

# Input and output file paths
input_file = "misc/song.txt"
output_file = "misc/song_new.txt"

# Translate the file
translate_file(input_file, output_file)
