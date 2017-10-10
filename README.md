# ircontrol
infrared controller written in C++ using Qt. Allows you to bind any button on an infrared controller (tv remote, universal remote etc) to an external script or application. Database storage using sqlite3 is in the works, currently, your binds are not saved.

I personally have two binds permanently bound, one to pause cmus, another to run a shell script that turns my monitor off.

Scripts/applications are NON blocking when executed, which means you can run as many as you'd like without the application stopping while they execute.

To use this, you must first setup an Arduino micro-controller to send received infrared signals through serial. Many tutorials can be found online, like http://www.instructables.com/id/Arduino-Infrared-Remote-tutorial/. The arduino code only has to send raw values, nothing else.

## TODO
- finish implementing database storage
- implement database retrieving
- implement bind deletion, mirror the results to the ui

![example image](https://raw.githubusercontent.com/daniel-Jones/ircontrol/master/image.png)

