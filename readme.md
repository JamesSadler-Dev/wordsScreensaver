
<h1>Random Word Screensaver</h1>
A screensaver for windows which displays centered and randomized words, with random colors and locations. This screensaver comes with a utility to format your text for use therein.
<h4><b>Installation:</b></h4>
<ol>
<li>
Use textformatter.py to format your input tokens, tab separated and newline separated.

textformatter usage:
```ps
python ./scripts/textformatter.py <path to input file> <desired output file>
```
Raises `IllegalFileInputError` if input file is not found
</li>
<hr><br>
<li>
<b>Compilation:</b>
use Visual Studio with MVSC compiler or Cmake manually-<br>
Within randomwordScreensaver.cpp you create an instance of a screensaver object to be used in the executable. There are a few parameters you can set upon instantiaton- 


```cpp
wordScreensaver scr = wordScreensaver(130,"words.txt");
scr.run();
```

The first parameter of the constructor being `fontsize`, which is how you can set the screensaver to scale to any size.<br><br>
The latter parameter sets the `filename` member of the screensaver class, which is the file that your newly compiled screensaver will be expecting. If your file is not found an error will be displayed and the screensaver will close after three seconds.
<br>
<br>
<hr>
</li>
<li>
<b>Screensaver: </b>Copy exe to screensaver file<br>

in powershell or WSL:

```ps
cp .\randomwordScreensaver.exe .\randomwordScreensaver.scr
```


Right click `install` on randomwordscreensaver.scr<br><br>
Move your expected words file to `C:\windows\system32` to finish installation, this filename parameter should match the filename you have compiled your screensaver with.
</li>
</ol>
