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
                Shapes/arrow.h \
                Shapes/circle.h \
                Shapes/grid.h \
                Shapes/line.h \
                action.h \
                manager.h \
                maths.h \
                scribblearea.h \
                state.h \
                uiEventHandlers/stateplacement.h \
                view.h

SOURCES       = main.cpp \
                Astar/Agent.cpp \
                Astar/Astar.cpp \
                Astar/MAPPGridState.cpp \
                Astar/heuristic.cpp \
                Shapes/arrow.cpp \
                Shapes/circle.cpp \
                Shapes/grid.cpp \
                Shapes/line.cpp \
                action.cpp \
                mainwindow.cpp \
                manager.cpp \
                maths.cpp \
                mouseMethods.cpp \
                scribblearea.cpp \
                state.cpp \
                uiEventHandlers/stateplacement.cpp \
                view.cpp

# install
target.path = $$/home/aod/Desktop/github/StateMachineGenerator/UI
INSTALLS += target

DISTFILES +=
