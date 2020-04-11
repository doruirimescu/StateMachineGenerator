QT += widgets
requires(qtConfig(filedialog))
qtHaveModule(printsupport): QT += printsupport

HEADERS       = mainwindow.h \
                PropertyHelper.h \
                action.h \
                manager.h \
                maths.h \
                scribblearea.h \
                state.h \
                view.h
SOURCES       = main.cpp \
                action.cpp \
                mainwindow.cpp \
                manager.cpp \
                maths.cpp \
                mouseMethods.cpp \
                scribblearea.cpp \
                state.cpp \
                view.cpp

# install
target.path = $$/home/aod/Desktop/github/StateMachineGenerator/UI
INSTALLS += target
