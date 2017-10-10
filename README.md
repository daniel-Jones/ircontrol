# ircontrol
infrared controller written in C++ using Qt. Allows you to bind any button on an infrared controller (tv remote, universal remote etc) to an external script or application.

I personally have two binds permanently bound, one to pause cmus, another to run a shell script that turns my monitor off.

To use this, you must first setup an Arduino micro-controller to send received infrared signal through serial. Many tutorials can be found online, like http://www.instructables.com/id/Arduino-Infrared-Remote-tutorial/. The arduino code only has to send raw values, nothing else.

![example image](placeholder)

