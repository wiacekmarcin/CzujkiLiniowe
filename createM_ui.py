
txt="""
<item row="%(row)d" column="0">
    <widget class="QLabel" name="lnazwa_%(row)d">
        <property name="text">
        <string>M%(row)d</string>
        </property>
    </widget>
</item>
<item row="%(row)d" column="1">
    <widget class="QLineEdit" name="nazwa_%(row)d"/>
</item>
<item row="%(row)d" column="2">
    <widget class="QLineEdit" name="ratio_%(row)d"/>
</item>
<item row="%(row)d" column="3">
    <widget class="QLabel" name="lunit1_%(row)d">
        <property name="text">
        <string>*/kroki</string>
        </property>
    </widget>
</item>
<item row="%(row)d" column="4">
    <widget class="QLineEdit" name="speedMax1_%(row)d"/>
</item>
<item row="%(row)d" column="5">
    <widget class="QLabel" name="lunit2_%(row)d">
        <property name="text">
        <string>*/min</string>
        </property>
    </widget>
</item>
<item row="%(row)d" column="6">
    <widget class="QLineEdit" name="maxSpeed2_%(row)d"/>
</item>
<item row="%(row)d" column="7">
    <widget class="QLabel" name="lunit3_%(row)d">
        <property name="text">
        <string>krok/s</string>
        </property>
    </widget>
</item>
<item row="%(row)d" column="8">
    <widget class="QCheckBox" name="reverse_%(row)d">
     <property name="text">
      <string>Tak</string>
     </property>
    </widget>
</item>
<item row="%(row)d" column="9">
    <widget class="QLineEdit" name="maxSteep_%(row)d"/>
</item>"""


for i in range(1, 10):
    print(txt % { 'row' : i })
