This is a simple app with a simple purpose. It will watch a file for you, and re-display the contents (be it text or image data) when the file is updated.

Example usage scenarios:

* You're writing an app that dumps internal state to separate files (say, a compiler dumping ASTs, IR, and assembly listings). AllSeeingEye will give you an automatically-updated view after every compile!
* You are manually tweaking a GraphViz file, and are frustrated with the "edit -- save -- run dot -- switch-to-image-viewer -- open-new-image -- close-new-image" cycle. **AllSeeingEye** will eliminate the last few steps, displaying the newly-updated image as soon as dot spits it out.

**AllSeeingEye** nicely complements Ian Piumarta's [watch utility](http://piumarta.com/software/watch/). For example, `watch` can automatically run `dot` on a GraphViz file whenever you edit and save, and then **AllSeeingEye** will show the updated PNG (or whatever) file that `dot` spits out. Thus, the cycle above would be reduced to "edit -- save", and the output from dot would seamlessly update in the background whenever you hit save. No need to even leave your editor!

### Running ###

If you provide a file path as the first argument to AllSeeingEye on the command line, AllSeeingEye will immediately begin watching the file at startup. If you do not provide a file path, you may select a file to watch at any time with the "Watch File" button at the bottom of the window.

AllSeeingEye will automatically switch between the Text Watcher and Image Watcher tabs depending on the (initial) contents of the file being watched.

### Compiling ###

This code was originally written against Qt version 4. I haven't tried it with Qt 5. The Ubuntu packages were once called `libqt4-dev libqtgui4 libqtcore4`

On Linux, clone the repository, then a simple `qmake && make` should work.

On Mac OS X, build with `qmake -spec macx-g++ && make`.

To create a disk image:
```
#!sh
$ macdeployqt AllSeeingEye.app/ -dmg

# This step is important! Otherwise you'll get errors from Qt
# being doubly-loaded if you use Qt Creator to run the app.
$ rm -rf AllSeeingEye.app/
```