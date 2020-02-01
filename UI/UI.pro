QT += widgets
requires(qtConfig(filedialog))
qtHaveModule(printsupport): QT += printsupport

HEADERS       = mainwindow.h \
                action.h \
                component.h \
                manager.h \
                scribblearea.h \
                state.h
SOURCES       = main.cpp \
                action.cpp \
                component.cpp \
                mainwindow.cpp \
                manager.cpp \
                scribblearea.cpp \
                state.cpp

# install
target.path = $$/home/aod/Desktop/github/UI
INSTALLS += target
