# -*- coding: utf-8 -*-
xmlcode = u"""        <widget class="QWidget" name="p%(Uname)s">
         <property name="styleSheet">
          <string notr="true"/>
         </property>
         <layout class="QVBoxLayout" name="verticalLayoutp%(Uname)s">
          <property name="spacing">
           <number>0</number>
          </property>
          <property name="leftMargin">
           <number>0</number>
          </property>
          <property name="topMargin">
           <number>0</number>
          </property>
          <property name="rightMargin">
           <number>0</number>
          </property>
          <property name="bottomMargin">
           <number>0</number>
          </property>
          <item>
           <widget class="QWidget" name="%(sname)sWidget" native="true">
            <property name="sizePolicy">
             <sizepolicy hsizetype="Preferred" vsizetype="Minimum">
              <horstretch>0</horstretch>
              <verstretch>0</verstretch>
             </sizepolicy>
            </property>
            <property name="minimumSize">
             <size>
              <width>0</width>
              <height>70</height>
             </size>
            </property>
            <property name="maximumSize">
             <size>
              <width>16777215</width>
              <height>60</height>
             </size>
            </property>
            <layout class="QFormLayout" name="formLayout%(sname)sWidget">
             <property name="labelAlignment">
              <set>Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop</set>
             </property>
             <item row="0" column="0">
              <widget class="QLabel" name="e%(Uname)snazwaTestu">
               <property name="font">
                <font>
                 <bold>true</bold>
                </font>
               </property>
               <property name="text">
                <string>Nazwa testu:</string>
               </property>
              </widget>
             </item>
             <item row="0" column="1">
              <widget class="QLabel" name="%(sname)snazwaTestu">
               <property name="maximumSize">
                <size>
                 <width>16777215</width>
                 <height>35</height>
                </size>
               </property>
               <property name="frameShape">
                <enum>QFrame::StyledPanel</enum>
               </property>
               <property name="text">
                <string>Kompatybilność elektromagnetyczna EMC - wyładowania elektrostatyczne (Electromagnetic compatibility EMC - electrostatic discharge)</string>
               </property>
              </widget>
             </item>
             <item row="1" column="1">
              <widget class="QPushButton" name="pb%(Uname)s">
               <property name="text">
                <string>Test nie został jeszcze wykonany, kliknij aby go wykonać</string>
               </property>
              </widget>
             </item>
            </layout>
           </widget>
          </item>
          <item>
           <widget class="QWidget" name="%(sname)sWyniki" native="true">
            <layout class="QFormLayout" name="formLayout%(sname)sWyniki">
             <item row="0" column="0">
              <widget class="QLabel" name="et%(Uname)sResult">
               <property name="font">
                <font>
                 <bold>true</bold>
                </font>
               </property>
               <property name="text">
                <string>Wynik</string>
               </property>
              </widget>
             </item>
             <item row="0" column="1">
              <widget class="QLabel" name="%(sname)sResult">
               <property name="frameShape">
                <enum>QFrame::WinPanel</enum>
               </property>
               <property name="frameShadow">
                <enum>QFrame::Sunken</enum>
               </property>
               <property name="text">
                <string>POZYTYWNY NEGATYWNY</string>
               </property>
               <property name="margin">
                <number>2</number>
               </property>
               <property name="indent">
                <number>5</number>
               </property>
              </widget>
             </item>
             <item row="1" column="0">
              <widget class="QLabel" name="et%(sname)sParametry">
               <property name="font">
                <font>
                 <bold>true</bold>
                </font>
               </property>
               <property name="text">
                <string>Parametry</string>
               </property>
              </widget>
             </item>
             <item row="1" column="1">
              <widget class="QTableWidget" name="%(sname)sTableParams">
               <property name="sizePolicy">
                <sizepolicy hsizetype="Minimum" vsizetype="Minimum">
                 <horstretch>0</horstretch>
                 <verstretch>0</verstretch>
                </sizepolicy>
               </property>
               <property name="maximumSize">
                <size>
                 <width>290</width>
                 <height>97</height>
                </size>
               </property>
               <property name="frameShape">
                <enum>QFrame::WinPanel</enum>
               </property>
               <property name="verticalScrollBarPolicy">
                <enum>Qt::ScrollBarAsNeeded</enum>
               </property>
               <property name="horizontalScrollBarPolicy">
                <enum>Qt::ScrollBarAsNeeded</enum>
               </property>
               <property name="autoScroll">
                <bool>false</bool>
               </property>
               <property name="editTriggers">
                <set>QAbstractItemView::NoEditTriggers</set>
               </property>
               <property name="tabKeyNavigation">
                <bool>false</bool>
               </property>
               <property name="showDropIndicator" stdset="0">
                <bool>false</bool>
               </property>
               <property name="dragDropOverwriteMode">
                <bool>false</bool>
               </property>
               <property name="alternatingRowColors">
                <bool>true</bool>
               </property>
               <attribute name="horizontalHeaderVisible">
                <bool>false</bool>
               </attribute>
               <row>
                <property name="text">
                 <string>Cmin</string>
                </property>
               </row>
               <row>
                <property name="text">
                 <string>Cmax</string>
                </property>
               </row>
               <row>
                <property name="text">
                 <string>Cmax/Cmin</string>
                </property>
               </row>
               <column>
                <property name="text">
                 <string>Nazwa</string>
                </property>
               </column>
               <column>
                <property name="text">
                 <string>Wartość</string>
                </property>
               </column>
               <item row="0" column="0">
                <property name="text">
                 <string>0</string>
                </property>
               </item>
               <item row="0" column="1">
                <property name="text">
                 <string>0</string>
                </property>
               </item>
               <item row="1" column="0">
                <property name="text">
                 <string>0</string>
                </property>
               </item>
               <item row="1" column="1">
                <property name="text">
                 <string>0</string>
                </property>
               </item>
               <item row="2" column="0">
                <property name="text">
                 <string>0</string>
                </property>
               </item>
               <item row="2" column="1">
                <property name="text">
                 <string>0</string>
                </property>
               </item>
              </widget>
             </item>
             <item row="5" column="1">
              <widget class="QLabel" name="%(sname)sWynikNarazenia">
               <property name="text">
                <string/>
               </property>
              </widget>
             </item>
             <item row="7" column="1">
              <spacer name="verticalSpacer%(Uname)s">
               <property name="orientation">
                <enum>Qt::Vertical</enum>
               </property>
               <property name="sizeHint" stdset="0">
                <size>
                 <width>20</width>
                 <height>40</height>
                </size>
               </property>
              </spacer>
             </item>
             <item row="3" column="0">
              <widget class="QLabel" name="et%(Uname)sCzujka">
               <property name="font">
                <font>
                 <bold>true</bold>
                </font>
               </property>
               <property name="text">
                <string>Czujka</string>
               </property>
              </widget>
             </item>
             <item row="3" column="1">
              <widget class="QTableWidget" name="%(sname)stableCzujka">
               <property name="minimumSize">
                <size>
                 <width>309</width>
                 <height>70</height>
                </size>
               </property>
               <property name="maximumSize">
                <size>
                 <width>330</width>
                 <height>70</height>
                </size>
               </property>
               <property name="editTriggers">
                <set>QAbstractItemView::NoEditTriggers</set>
               </property>
               <row>
                <property name="text">
                 <string>1</string>
                </property>
               </row>
               <column>
                <property name="text">
                 <string>Nadajnik</string>
                </property>
               </column>
               <column>
                <property name="text">
                 <string>Odbiornik</string>
                </property>
               </column>
              </widget>
             </item>
             <item row="2" column="0">
              <widget class="QLabel" name="et%(Uname)sWynikNarazenia">
               <property name="font">
                <font>
                 <bold>true</bold>
                </font>
               </property>
               <property name="text">
                <string>Narażenie</string>
               </property>
              </widget>
             </item>
             <item row="2" column="1">
              <widget class="QTableWidget" name="et%(sname)sNarazenie">
               <property name="minimumSize">
                <size>
                 <width>500</width>
                 <height>70</height>
                </size>
               </property>
               <property name="maximumSize">
                <size>
                 <width>500</width>
                 <height>71</height>
                </size>
               </property>
               <property name="editTriggers">
                <set>QAbstractItemView::NoEditTriggers</set>
               </property>
               <row>
                <property name="text">
                 <string>1</string>
                </property>
               </row>
               <column>
                <property name="text">
                 <string>Wynik narażenia</string>
                </property>
               </column>
               <column>
                <property name="text">
                 <string>Rodzaj narażenia</string>
                </property>
               </column>
               <column>
                <property name="text">
                 <string>Uwagi</string>
                </property>
               </column>
              </widget>
             </item>
             <item row="4" column="1">
              <widget class="QTableWidget" name="%(sname)sTablePrzebieg">
               <property name="minimumSize">
                <size>
                 <width>395</width>
                 <height>160</height>
                </size>
               </property>
               <property name="maximumSize">
                <size>
                 <width>430</width>
                 <height>150</height>
                </size>
               </property>
               <property name="frameShape">
                <enum>QFrame::WinPanel</enum>
               </property>
               <property name="sizeAdjustPolicy">
                <enum>QAbstractScrollArea::AdjustToContents</enum>
               </property>
               <property name="autoScroll">
                <bool>false</bool>
               </property>
               <property name="editTriggers">
                <set>QAbstractItemView::NoEditTriggers</set>
               </property>
               <property name="tabKeyNavigation">
                <bool>false</bool>
               </property>
               <property name="showDropIndicator" stdset="0">
                <bool>false</bool>
               </property>
               <property name="dragDropOverwriteMode">
                <bool>false</bool>
               </property>
               <property name="alternatingRowColors">
                <bool>true</bool>
               </property>
               <property name="selectionMode">
                <enum>QAbstractItemView::NoSelection</enum>
               </property>
               <property name="wordWrap">
                <bool>false</bool>
               </property>
               <property name="cornerButtonEnabled">
                <bool>false</bool>
               </property>
               <attribute name="horizontalHeaderCascadingSectionResizes">
                <bool>true</bool>
               </attribute>
               <attribute name="horizontalHeaderMinimumSectionSize">
                <number>130</number>
               </attribute>
               <attribute name="verticalHeaderVisible">
                <bool>true</bool>
               </attribute>
               <attribute name="verticalHeaderCascadingSectionResizes">
                <bool>true</bool>
               </attribute>
               <attribute name="verticalHeaderShowSortIndicator" stdset="0">
                <bool>false</bool>
               </attribute>
               <attribute name="verticalHeaderStretchLastSection">
                <bool>false</bool>
               </attribute>
               <row>
                <property name="text">
                 <string>1</string>
                </property>
               </row>
               <column>
                <property name="text">
                 <string>C[n]</string>
                </property>
               </column>
               <column>
                <property name="text">
                 <string>C[n]</string>
                </property>
               </column>
               <column>
                <property name="text">
                 <string>Uwagi</string>
                </property>
               </column>
              </widget>
             </item>
             <item row="4" column="0">
              <widget class="QLabel" name="et%(Uname)sPrzebieg">
               <property name="font">
                <font>
                 <bold>true</bold>
                </font>
               </property>
               <property name="text">
                <string>Przebieg</string>
               </property>
              </widget>
             </item>
            </layout>
           </widget>
          </item>
         </layout>
        </widget>"""

func = lambda s: s[:1].lower() + s[1:] if s else ''

def printonePage(nazwa):
    d = {}
    d['Uname'] = nazwa
    d['sname'] = func(nazwa)
    print(xmlcode % d)

nazwy = ['EMC_wyladowaniaelektrostatyczne', 'EMC_promieniowaniepolaelektromagnetycznego']

['EMC_zakloceniaindukowaneprzezpoleelektromagnetyzne', 'EMC_szybkiekrotkieblyski',
'EMC_powolneSkokiNapieciaDuzejEnergii']
#for n in nazwy:
#  printonePage(n)

printonePage('EMC_szybkiekrotkieblyski')
printonePage('EMC_powolneSkokiNapieciaDuzejEnergii')




