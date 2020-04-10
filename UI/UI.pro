QT += widgets
requires(qtConfig(filedialog))
qtHaveModule(printsupport): QT += printsupport

HEADERS       = mainwindow.h \
                PropertyHelper.h \
                action.h \
                component.h \
                manager.h \
                scribblearea.h \
                state.h
SOURCES       = main.cpp \
                component.cpp \
                mainwindow.cpp \
                manager.cpp \
                scribblearea.cpp \
                state.cpp

# install
target.path = $$/home/aod/Desktop/github/StateMachineGenerator/UI
INSTALLS += target
