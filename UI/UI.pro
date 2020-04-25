QT += widgets
requires(qtConfig(filedialog))
qtHaveModule(printsupport): QT += printsupport

HEADERS       = mainwindow.h \
                Astar/Agent.hpp \
                Astar/Astar.hpp \
                Astar/MAPPGridState.hpp \
                Astar/Wall.hpp \
                Astar/heuristic.hpp \
                Astar/porting.hpp \
                PropertyHelper.h \
                action.h \
                manager.h \
                maths.h \
                scribblearea.h \
                state.h \
                view.h
SOURCES       = main.cpp \
                Astar/Agent.cpp \
                Astar/Astar.cpp \
                Astar/MAPPGridState.cpp \
                Astar/heuristic.cpp \
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
