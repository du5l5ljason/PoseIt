
Overview:

The project "Pose it" is an interactive video game, which is designed to producing and sharing happiness through making and resembling "weird" poses.

Sensing Module: Kinect 3D camera + OpenNI library.
Motion Analysis: We can track as many as 24 joints from the skeleton data of one user. Thus, by analyzing the joint positions and angles in different scenes, we can calculate the similarity between two different poses.
Interaction Design:
1. Control: Use hand gesture, instead of mouse and keyboard to control. This is implemented by using hand gesture API in NITE library.
2. Rules: The players are devided into two groups. First, the first group is required to make a pose. Then, the second group is required to resemble it within seconds (). The game is continued by switching the characters between two groups.
3. Feedback: No audio feedback or video feedback, but we are interested in adding some video feedback, such as showing which parts of body is not correct...
