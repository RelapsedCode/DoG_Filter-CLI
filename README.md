# DoG_Filter-CLI
## Project overview
CLI application applies Difference of Gaussian filter on top of an image. It is used for quickly generating a lot of images without visualizing the image currently in progress.
___
## How to use
Input the minimum sigma values from which the application will start generating the new images applying +5 for the first sigma and +1 for the second. The procedure will stop when sigma 1 reaches 40.
___
![Zero-crossing](https://github.com/RelapsedCode/DoG_Filter-CLI/blob/master/demo-images/zero-crossing-th.gif | width=1920px)
![Zero-crossing+image](https://github.com/RelapsedCode/DoG_Filter-CLI/blob/master/demo-images/contours.gif)
___
## OpenCV installation
https://medium.com/@subwaymatch/opencv-410-with-vs-2019-3d0bc0c81d96
https://docs.opencv.org/master/dd/d6e/tutorial_windows_visual_studio_opencv.html

Look for the opencv dlls in: project_folder\x64\Debug
Files opencv_world440d.dll and opencv_world440.dll (440 is the version, can be different) should be there, if not paste them from c:\opencv
___
Final result after using image editors to polish the output and separating the objects:
![](https://github.com/RelapsedCode/DoG_Filter-CLI/blob/master/demo-images/layers.gif)
