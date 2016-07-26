# WordOCR
How hard can it be to make a computer to recognize text from pictures? As a part of Data Mining course in Fall 2009, me and [Jaana](https://www.linkedin.com/in/jaanametsamaa) implemented a small program that extracted letters from pictures and then recognised them.

## This repostiory contains
* [Project report](../master/wordocr.pdf)
* [Presentation](http://www.slideshare.net/kauralasoo/building-a-naive-ocr-system)
* Pictures of [242 words](../master/data/words.zip) that were used in the experiments
* OpenCV C++ [code](../master/code/main.cpp) used for image preprocessing and feature extraction
* Clean [data matrix](../master/data/letters.csv) for machine learning
  * Column 1: character from a-z
  * Column 2: aspect ratio (height divided by width)
  * Columns 3-130: 16 x 8 normalized pixel values
