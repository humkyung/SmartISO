Sub main()
    Dim startPoint As MbePoint
    Dim point As MbePoint, point2 As MbePoint
    Dim S$, Values(4) As Double, numArgs as Integer
    Dim strArgs as String , cmdArg as String
    
    'save the number of arguments in a local variable
    numArgs = WordCount(Command$)

    'output the number of command line arguments
    MbeMessageBox "Number of command line arguments is : " & Str$(numArgs)

    '   parse the command line arguments and output each individual argument in turn
    For counter% = 1 to numArgs
        cmdArg = Word$ (Command$, counter)
        Values(counter - 1) = Val(cmdArg)
        MbeMessageBox "Argument #" & Str$(counter) & " is " & cmdArg
    Next counter

'   Start a command
    MbeSendCommand "DIALOG TOOLBOX FENCE TOGGLE "
    MbeSendCommand "PLACE FENCE ICON "
 
'   Coordinates are in master units
    startPoint.x = Values(0)
    startPoint.y = Values(1)
    startPoint.z = 0#

'   Send points to simulate a down-drag-up action
    point.x = startPoint.x
    point.y = startPoint.y
    point.z = startPoint.z
    point2.x = Values(2)
    point2.y = Values(3)
    point2.z = point.z
    MbeSendDragPoints point, point2, 1

    'MbeSendCommand "place fence element "
    'MbeSendCommand "plot entity fence "

    MbeSendCommand "DIALOG PLOT;PLOT ENTITY FENCE "
    'change pen table
    MbeSendCommand "_PLOT _PENTABLE _LOAD "
    MbeSendCommand "MDL COMMAND MGDSHOOK,fileList_setFileNameCmd SmartISO.tbl"
    MbeSendCommand "MBE1 CLOSEMODAL OK"
    MbeSendCommand "pentable load SmartISO.tbl"
    'up to here

    'use default printer
    'MbeSendCommand "plot plotter Adobe PDF "
    'MbeSendCommand "plot paper rotate on "
    'MbeSendCommand "plot paper name A3 "

    MbeSendCommand "plot margins center "
    MbeSendCommand "plot size maximize "

    MbeSendCommand "plot border off "
    MbeSendCommand "plot options fenceboundary off "
    
    MbeSendCommand "plot execute "
    
End Sub

 
