#!/bin/sh

pandoc -t revealjs -s -o index.html slides.md -V revealjs-url=https://unpkg.com/reveal.js/

