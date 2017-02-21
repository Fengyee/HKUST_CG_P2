# HKUST_CG_P2
###### 　By FENG Haoan (Fengyee) and ZHAO Lucen (ZhaoLucen). 2017 Spring
***
### To Do List: 
> 
> #### Project Requirements
> 
> First of all, you must come up with a character. This character can be composed solely of primitive shapes (box, generalized cylinder, sphere, and triangle).  It should use at least ten primitives and at least four levels of hierarchy. You must also use at least one each of the `glTranslate()`, `glRotate()` and `glScale()` calls to position these primitives in space (and you will probably use many of all of them!) You must also use `glPushMatrix()` and `glPopMatrix()` to nest your matrix transformations. The modeler skeleton provides functions for creating sliders and hooking them to different features of your model. You must add at least one of these as a control knob (slider, actually) for some joint/component of your model - have your character do some simple action as you scrub a slider back and forth. In addition, at least one of your controls must be tied to more than one joint/component; this knob will change the input to some function that determines how several parts of your model can move. For example, in a model of a human, you might have a slider that straightens a leg by extending both the knee and hip joints.
> 
> In the Camera class, the function called `applyViewingTransform()` uses `gluLookAt()` to perform a viewing transform that effectively moves the camera to the specified position and orientation. You are required to implement your own version of `gluLookAt()` using any method (i.e. the standard transformations (glTranslate, glRotate, glScale), matrix construction) as long as you are not using the gluLookAt itself. Add a function to the Camera class with the following prototype:
> 
> `void Camera::lookAt(double *eye, double *at, double *up)`

> where eye, at and up are each 3 element arrays that specify the position of the camera, the point it is looking at and the up vector in world co-ordinates. Using these arguments, your function should apply a series of translations and rotations (or construct a matrix) that effectively moves the camera to the specified position and orientation.
> > 
> > You are required to complete two bells (or equivalent) from the list below. Feel free to come up with your own ideas and pass them by us. If they're cool, we'll count them as bells and whistles.
> > 
> > Other than these requirements, you have complete artistic freedom on this, so be creative!