Attribute VB_Name = "Module1"

Function BitAnd(one As Integer, Two As Integer) As Integer
    BitAnd = one And Two
End Function


Function MMD(Term As Integer, Control As Integer, Mask As Integer) As Integer
    x = Control And Term
    MMD = Term
    
    If (Control = x) Then
        MMD = Term Xor Mask
    End If
    
End Function
