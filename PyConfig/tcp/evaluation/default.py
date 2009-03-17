from openwns.evaluation import *

def installEvaluation(sim):
    sourceNodeName = 'tcp.cumulativeACK.windowSize'
    node = openwns.evaluation.createSourceNode(sim, sourceNodeName)
    node.appendChildren(TimeSeries())

    sourceNodeName = 'tcp.cumulativeACK.sendCredit'
    node = openwns.evaluation.createSourceNode(sim, sourceNodeName)
    node.appendChildren(TimeSeries())
