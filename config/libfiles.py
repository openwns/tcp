libname = 'tcp'
srcFiles = [
    'src/Module.cpp',
    'src/Component.cpp',
    'src/Service.cpp',
    'src/TCPHeader.cpp',
    'src/IPDataHandler.cpp',
    'src/FlowIDBuilder.cpp',
    'src/Connection.cpp',
    'src/UpperConvergence.cpp',
    'src/ConnectionControl.cpp',
    'src/LowerConvergence.cpp',
    'src/FiniteStateMachine.cpp',
    'src/OneWayHandshake.cpp',
    'src/ThreeWayHandshake.cpp',
    'src/CumulativeACK.cpp',
    'src/CongestionControl.cpp',
    'src/SlowStart.cpp',
    'src/TahoeCongAvoid.cpp',

    'src/tests/CongestionControlTest.cpp',
    #'src/tests/CumulativeACKTest.cpp',
    ]
hppFiles = [
'src/ThreeWayHandshake.hpp',
'src/Module.hpp',
'src/CumulativeACK.hpp',
'src/ConnectionHandler.hpp',
'src/DataHandler.hpp',
'src/Service.hpp',
'src/CongestionControlStrategy.hpp',
'src/SlowStart.hpp',
'src/FlowIDBuilder.hpp',
'src/LowerConvergence.hpp',
'src/Connection.hpp',
'src/OneWayHandshake.hpp',
'src/TCPHeader.hpp',
'src/UpperConvergence.hpp',
'src/ConnectionControl.hpp',
'src/IPDataHandler.hpp',
'src/HandshakeStrategyInterface.hpp',
'src/FiniteStateMachine.hpp',
'src/Component.hpp',
'src/HandshakeStrategyHandlerInterface.hpp',
'src/TahoeCongAvoid.hpp',
'src/CongestionControl.hpp',
]

pyconfigs = [
'tcp/TCP.py',
'tcp/__init__.py',
'tcp/evaluation/__init__.py',
'tcp/evaluation/default.py',
]
dependencies = []
Return('libname srcFiles hppFiles pyconfigs dependencies')
