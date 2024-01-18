# digital-photo-album

Completed for Operating Systems Class

## What it Does

This project creates a binary, `albun`, which allows a user to peruse a set of large images downloaded from a digital camera, and produce an html photo album.
It uses a background process to resize images in the background, while correctly pipeling functionality which depend on user input.
After the child background process has finished resizing a specific image, it sends the `argv` index of the image to a inter-process pipe, letting the parent process know that it can display the thumbnail and continue with the user-side functionality.

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

## Modules

All header files are in the `include` directory.

### Image

The image module provides an API which wrapps ImageMagick commands in fork/exec calls.
It waits for the conversion processes to finish to prevent fork bombs when converting in mass.

### Util

A general utility module which provides APIs for resizing all images or processing an individual image with user input.
This module also provides the API for getting the appropriate filenames, which is used both internally and in the HTML module

### Html

The html module handles formatting the html photo album in a buffer string in memory, and then writes the file to disk through an API call.
The html buffer is appropriately resized when necessary to avoid buffer overflows.

## Limitations

Known Bug: if the user enters EOF (ctrl-d) then the input loops infinitely...there is an error within get_input.
