 #!/usr/bin/python3
from StateHeaderGenerator import stateheader
from StateCppGenerator import statecpp
from StateMachineHeaderGenerator import machineheader
from StateMachineCppGenerator import machinecpp
className = "State"

#   Label                           Left                        Right                    Up                 Down                            Select
a=[ ( "Menu 1",                     "Menu 2",                   -1,                     -1,                 "Menu1 Feature 1",             "Enter game1"),
    ( "Menu 2",                     "Menu 3",                   -1,                     -1,                 "Menu2 Feature 1",             -1),
    ( "Menu 3",                     -1,                         -1,                     -1,                 "Menu3 Feature 1",             -1),
    ( "Menu1 Feature 1",            "Menu1 Feature 2",          -1,                     -1,                 "Menu1 Feature1 Subfeature 1", -1),
    ( "Menu1 Feature 2",            -1,                         -1,                     -1,                 -1,                            -1),
    ( "Menu1 Feature1 Subfeature 1",-1,                         -1,                     -1,                 -1,                            -1),
    ( "Menu3 Feature 1",            -1,                         -1,                     -1,                 -1,                            -1),
    ( "Menu2 Feature 1",            -1,                         -1,                     -1,                 -1,                            -1),
    ( "Enter game1",                -1,                         -1,                     -1,                 -1,                            -1)
   ]

outputs = [ "cout<<\"1\"<<endl",
            "cout<<\"2\"<<endl",
            "cout<<\"3\"<<endl",
            "cout<<\"4\"<<endl",
            "cout<<\"5\"<<endl",
            "cout<<\"6\"<<endl",
            "cout<<\"7\"<<endl",
            "cout<<\"8\"<<endl",
            "cout<<\"Game1\"<<endl"
          ]
actionList      = ("Left",  "Right","Up",   "Down", "Select")
revActionList   = ("Right", "Left", "Down", "Up",   -1  )

class State:
    def __init__( self, index, label, successors, output ):
        self.index = index
        self.label = label
        self.output = output
        self.successors = successors

    def __repr__(self):
        return "Label: {} Index: {} Successors: ".format( self.label, 
                                            str(self.index), self.successors )

    def addNode(self):
        return '\n\t{Class} state{name}({state},\"{label}\");'.format( 
                    Class = className, name = str(self.index), 
                    label=self.label, state=str(self.index) )

    def addTransition(self):
        ret =""
        for action in self.successors:
            ret+= "\n\tstate{}.add{}(&state{});".format( str(self.index), 
                                            action, self.successors[ action ] )
        return ret

fc = list( x[0] for x in a )
stateList = list()

for i, row in enumerate(a):
    successors = dict()
    for j, field in enumerate( row[ 1 : len(actionList) + 1 ] ):
        if -1 is not field:
            successors.update( { actionList[j] : fc.index(field) } )
        else:
            continue
    s = State( i, row[ 0 ], successors, outputs[ i ] )
    stateList.append( s )

stateheader(actionList)
statecpp(actionList, revActionList)
machineheader(actionList, stateList)
machinecpp(actionList, stateList)