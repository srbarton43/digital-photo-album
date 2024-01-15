# digital-photo-album

Programming Assignment 1 for COSC58

## What it Does

This project creates a binary, `albun`, which allows a user to peruse a set of large images downloaded from a digital camera, and produce an html photo album.
It uses multiple processes to resize images in the background, while correctly pipeling functionality which depend on user input.

## Building

Run `make` in the home directory.

```bash
make
```

## Usage

Call executable `album` and list photos as arguments.
Then, the user must respond to both rotation and caption prompts in the terminal for the displayed image.
After finishing this step for each image, an html file, `index.html`, will be placed in the directory where album was called, including all necessary image files.

Eg.

```bash
./album photos/1.jpg photos/2.jpg photos/3.jpg other/directory/4.jpg
```

The resulting file structure would look like:

```md
.
|-- index.html
|-- med_1.jpg
|-- med_2.jpg
|-- med_3.jpg
|-- med_4.jpg
|-- other
|   `-- directory
|       `-- 4.jpg
|-- photos
|   |-- 1.jpg
|   |-- 2.jpg
|   |-- 3.jpg
|-- tn_1.jpg
|-- tn_2.jpg
|-- tn_3.jpg
`-- tn_4.jpg
```

## Limitations

Known Bug: if the user enters EOF (ctrl-d) then the input loops infinitely...there is an error within get_input.
