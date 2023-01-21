# -*- coding: utf-8 -*-
xmlcode = u"""     <widget class="QWidget" name="%(nazwa)s">
      <layout class="QGridLayout" name="gridLayout%(nazwa)s">
       <item row="1" column="1">
        <widget class="QFrame" name="%(nazwa)sfrcmax">
         <property name="frameShape">
          <enum>QFrame::StyledPanel</enum>
         </property>
         <property name="frameShadow">
          <enum>QFrame::Raised</enum>
         </property>
         <layout class="QHBoxLayout" name="horizontalLayout%(nazwa)scmax">
          <item>
           <widget class="QLabel" name="%(nazwa)sCmax">
            <property name="font">
             <font>
              <pointsize>14</pointsize>
             </font>
            </property>
            <property name="text">
             <string>0 dB</string>
            </property>
           </widget>
          </item>
          <item>
           <widget class="QLabel" name="%(nazwa)sCmax2">
            <property name="font">
             <font>
              <pointsize>14</pointsize>
             </font>
            </property>
            <property name="text">
             <string>0%%</string>
            </property>
           </widget>
          </item>
         </layout>
        </widget>
       </item>
       <item row="0" column="1">
        <widget class="QFrame" name="%(nazwa)sfrcmin">
         <property name="frameShape">
          <enum>QFrame::StyledPanel</enum>
         </property>
         <property name="frameShadow">
          <enum>QFrame::Raised</enum>
         </property>
         <layout class="QHBoxLayout" name="horizontalLayout%(nazwa)scmin">
          <item>
           <widget class="QLabel" name="%(nazwa)sCmin">
            <property name="font">
             <font>
              <pointsize>14</pointsize>
             </font>
            </property>
            <property name="text">
             <string>0 dB</string>
            </property>
           </widget>
          </item>
          <item>
           <widget class="QLabel" name="%(nazwa)sCmin2">
            <property name="font">
             <font>
              <pointsize>14</pointsize>
             </font>
            </property>
            <property name="text">
             <string>0%%</string>
            </property>
           </widget>
          </item>
         </layout>
        </widget>
       </item>
       <item row="2" column="0">
        <widget class="QLabel" name="e%(Nazwa2)sCmaxCmin">
         <property name="font">
          <font>
           <pointsize>14</pointsize>
          </font>
         </property>
         <property name="text">
          <string>Cmax/Cmin</string>
         </property>
        </widget>
       </item>
       <item row="1" column="0">
        <widget class="QLabel" name="e%(Nazwa2)sCmax">
         <property name="font">
          <font>
           <pointsize>14</pointsize>
          </font>
         </property>
         <property name="text">
          <string>Cmax</string>
         </property>
        </widget>
       </item>
       <item row="0" column="0">
        <widget class="QLabel" name="e%(Nazwa2)sCmin">
         <property name="font">
          <font>
           <pointsize>14</pointsize>
          </font>
         </property>
         <property name="text">
          <string>Cmin</string>
         </property>
         <property name="wordWrap">
          <bool>true</bool>
         </property>
        </widget>
       </item>
       <item row="2" column="1">
        <widget class="QLabel" name="%(nazwa)sCmaxCmin">
         <property name="font">
          <font>
           <pointsize>14</pointsize>
          </font>
         </property>
         <property name="frameShape">
          <enum>QFrame::Box</enum>
         </property>
         <property name="frameShadow">
          <enum>QFrame::Raised</enum>
         </property>
         <property name="text">
          <string>1</string>
         </property>
        </widget>
       </item>
       <item row="3" column="0" colspan="2">
        <widget class="QFrame" name="fr%(Nazwa2)sPrzebieg">
         <property name="frameShape">
          <enum>QFrame::StyledPanel</enum>
         </property>
         <property name="frameShadow">
          <enum>QFrame::Raised</enum>
         </property>
         <layout class="QGridLayout" name="%(nazwa)sPrzebiegGridLayout"/>
        </widget>
       </item>
      </layout>
     </widget>"""

func = lambda s: s[:1].lower() + s[1:] if s else ''

def printonePage(nazwa):
    d = {}
    d['nazwa'] = 'rozporoszoneswiatlo'
    d['Nazwa2'] = 'RozproszoneSwiatlo'

    print(d)
    print(xmlcode % d)

nazwa = 'TolerancjaNapieciaZasilania'
printonePage(nazwa)