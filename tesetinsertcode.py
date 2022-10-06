def createHead():
    for s in ["Ratio", "MaxSteps", "BaseSteps", "Delayus", "LeftRotation", "Enable"]:
        if s == "LeftRotation" or s == "Enable":
            v1 = "bool"
            v2 = "bool"
        else:
            v1 = "QString"
            v2 = "const QString &"
        print("\t%s get%sSilnik(uint silnik) const;\n\tvoid set%sSilnik(uint silnik, %s value);\n" %(v1,s,s,v2))
        for i in range(1, 10):
            print("\t%s get%sSilnik%d() const;\n\tvoid set%sSilnik%d(%s value);\n" %(v1,s,i,s,i,v2))
    print("private:")
    for s in ["Ratio", "MaxSteps", "BaseSteps", "Delayus", "LeftRotation"]:
        if s == "LeftRotation" or s == "Enable":
            v1 = "bool"
            v2 = "bool"
        else:
            v1 = "QString"
            v2 = "const QString &"
        v = s[0].lower() + s[1:]
        for i in range(1, 10):
            print("\t%s m_%sSilnik%d;" %(v1,v,i))
        
def createReadCode():
    for s in ["Ratio", "MaxSteps", "BaseSteps", "Delayus", "LeftRotation", "Enable"]:
        if s == "LeftRotation" or s == "Enable":
            v1 = "bool"
            v2 = "toBool"
        else:
            v1 = "QString"
            v2 = "toString"
        v = s[0].lower() + s[1:]
        for i in range(1, 10):
            print("\tm_%sSilnik%d = settings.value(\"%s_%d\", m_%sSilnik%d).%s();" %(v,i,s,i,v,i,v2))

def createWriteCode():
    for s in ["Ratio", "MaxSteps", "BaseSteps", "Delayus", "LeftRotation", "Enable"]:
        if s == "LeftRotation" or s == "Enable":
            v1 = "bool"
            v2 = "toBool"
        else:
            v1 = "QString"
            v2 = "toString"
        v = s[0].lower() + s[1:]
        for i in range(1, 10):
            print("\tsettings.setValue(\"%s_%d\", QVariant::fromValue(m_%sSilnik%d));" %(s,i,v,i))

def createSetGet():
    for s in ["Ratio", "MaxSteps", "BaseSteps", "Delayus", "LeftRotation", "Enable"]:
        if s == "LeftRotation" or s == "Enable":
            v1 = "bool"
            v2 = "bool"
            v3 = "false"
        else:
            v1 = "QString"
            v2 = "const QString &"
            v3 = "QString()"
        v = s[0].lower() + s[1:]
        print("%s Ustawienia::get%sSilnik(uint key) const" % (v1, s))
        print("{")
        print("    switch(key) {")
        for i in range(1, 10):            
            print("    case %d : return get%sSilnik%d();" % (i, s, i))
        print("    default: return %s;" % v3)
        print("    }\n}\n")
        
        print("void Ustawienia::set%sSilnik(uint key, %s value)" % (s, v2))
        print("{")
        print("    switch(key) {")
        for i in range(1, 10):            
            print("    case %d : set%sSilnik%d(value); break;" % (i, s, i))
        print("    default: break;")
        print("    }\n}\n")
        
        for i in range(1, 10):
            print("%s Ustawienia::get%sSilnik%d() const" % (v1, s, i))
            print("{")
            print("    return m_%sSilnik%d;" % (v,i))
            print("}\n")
            print("void Ustawienia::set%sSilnik%d(%s value)" % (s, i, v2))
            print("{")
            print("    m_%sSilnik%d = value;" % (v,i))
            print("    settings.setValue(\"%s_%d\", QVariant::fromValue(m_%sSilnik%d));" %(s,i,v,i))
            print("}\n")
            

#createHead()    
#createReadCode()
#createWriteCode()
createSetGet()

