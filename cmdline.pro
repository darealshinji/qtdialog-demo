QT           += widgets
QMAKE_LFLAGS += -s -Wl,--as-needed
INCLUDEPATH  += ../plugin
OBJECTS_DIR   = obj_cmdline
TARGET        = qtfiledialog
SOURCES       = plugin.cpp main.cpp

