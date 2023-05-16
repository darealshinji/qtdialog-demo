QT             += widgets
TEMPLATE        = lib
CONFIG         += plugin
QMAKE_LFLAGS   += -s -Wl,--as-needed
QMAKE_CXXFLAGS += -fvisibility=hidden
OBJECTS_DIR     = obj_plugin
TARGET          = qtfiledialog
SOURCES         = plugin.cpp

