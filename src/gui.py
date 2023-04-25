from PyQt5 import *
from PyQt5 import QtCore, QtGui, QtWidgets
import os,sys,subprocess
from PyQt5.QtWidgets import QApplication, QDialog, QLabel, QGroupBox, QVBoxLayout, QHBoxLayout, QCheckBox, QRadioButton
from PyQt5.QtWidgets import *
from PyQt5.QtCore import QRect
import qdarkstyle
from qdarkstyle.dark.palette import DarkPalette
import webbrowser

class Ui_MainWindow(object):
    pipeline = 0
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1920, 980)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.currentTheme = "L"
        # MainWindow.setStyleSheet("background-color: currentTheme;")
        
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(150, 0, 361, 41))
        font = QtGui.QFont()
        font.setPointSize(24)
        self.label.setFont(font)
        self.label.setObjectName("label")
        self.pushButton = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton.setGeometry(QtCore.QRect(0, 0, 89, 25))
        self.pushButton.setObjectName("pushButton")
        self.pushButton_2 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_2.setGeometry(QtCore.QRect(350, 70, 131, 31))
        self.pushButton_2.setObjectName("pushButton_2")
        self.pushButton_3 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_3.setGeometry(QtCore.QRect(425, 230, 131, 31))
        self.pushButton_3.setObjectName("pushButton_3")
        self.pushButton_4 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_4.setGeometry(QtCore.QRect(70, 750, 89, 25))
        self.pushButton_4.setObjectName("pushButton_4")
        self.toggleButton = QtWidgets.QPushButton(self.centralwidget)
        self.toggleButton.setGeometry(QtCore.QRect(430, 775, 110, 30))
        self.toggleButton.setObjectName("toggleButton")

        font = QtGui.QFont()
        self.tableWidget = QtWidgets.QTableWidget(self.centralwidget)  #for register data table 
        self.tableWidget.setGeometry(QtCore.QRect(290, 430, 256, 331)) #last index is to increase height 
        self.tableWidget.setRowCount(32)
        self.tableWidget.setColumnCount(1)
        self.tableWidget.setObjectName("tableWidget")

        self.line_3 = QtWidgets.QFrame(self.centralwidget)
        self.line_3.setGeometry(QtCore.QRect(870, 390, 2, 541))
        self.line_3.setFrameShape(QtWidgets.QFrame.VLine)
        self.line_3.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_3.setObjectName("line_3")
        self.line_3.setStyleSheet("background-color: black;")
        self.label_3 = QtWidgets.QLabel(self.centralwidget)
        self.label_3.setGeometry(QtCore.QRect(300, 390, 211, 21))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_3.setFont(font)
        self.label_3.setObjectName("label_3")
        self.label_4 = QtWidgets.QLabel(self.centralwidget)
        self.label_4.setGeometry(QtCore.QRect(680, 40, 211, 20))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_4.setFont(font)
        self.label_4.setObjectName("label_4")
        self.label_2 = QtWidgets.QLabel(self.centralwidget)
        self.label_2.setGeometry(QtCore.QRect(70, 50, 71, 17))
        self.label_2.setObjectName("label_2")
        self.textBrowser = QtWidgets.QTextBrowser(self.centralwidget) #text box of output log
        self.textBrowser.setGeometry(QtCore.QRect(10, 80, 271, 661))
        self.textBrowser.setObjectName("textBrowser")
        self.tableWidget_2 = QtWidgets.QTableWidget(self.centralwidget)
        self.tableWidget_2.setGeometry(QtCore.QRect(570, 70, 291, 741))
        self.tableWidget_2.setObjectName("tableWidget_2")
        self.tableWidget_2.setColumnCount(1)
        self.tableWidget_2.setRowCount(250000)
        
        self.pushButton_5 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_5.setGeometry(QtCore.QRect(285, 230, 131, 31))
        self.pushButton_5.setObjectName("pushButton_5")
        self.pushButton_6 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_6.setGeometry(QtCore.QRect(365, 120, 101, 31))
        self.pushButton_6.setObjectName("pushButton_6")
        self.textBrowser_2 = QtWidgets.QTextBrowser(self.centralwidget) #text box of assemble log
        self.textBrowser_2.setGeometry(QtCore.QRect(285, 160, 271, 61))
        self.textBrowser_2.setObjectName("textBrowser_2")

        self.label_5 = QtWidgets.QLabel(self.centralwidget)
        self.label_5.setGeometry(QtCore.QRect(300, 280, 81, 31))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_5.setFont(font)
        self.label_5.setObjectName("label_5")
        self.label_6 = QtWidgets.QLabel(self.centralwidget)
        self.label_6.setGeometry(QtCore.QRect(300, 330, 67, 17))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_6.setFont(font)
        self.label_6.setObjectName("label_6")

        self.line_4 = QtWidgets.QFrame(self.centralwidget)
        self.line_4.setGeometry(QtCore.QRect(870, 60, 2, 381))
        self.line_4.setFrameShape(QtWidgets.QFrame.VLine)
        self.line_4.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_4.setObjectName("line_4")
        self.line_4.setStyleSheet("background-color: black;")
        self.label_18 = QtWidgets.QLabel(self.centralwidget)
        self.label_18.setGeometry(QtCore.QRect(630, 80, 131, 20))
        font = QtGui.QFont()
        font.setPointSize(11)
        self.label_18.setFont(font)
        self.label_18.setObjectName("label_10")

        self.tableWidget.setColumnWidth(0,250) #here to resize the column width of tables
        self.tableWidget_2.setColumnWidth(0,270)
        MainWindow.setCentralWidget(self.centralwidget)
        font = QtGui.QFont()
        font.setPointSize(11)

        #for pipelining 
        # self.tableWidget_4 = QtWidgets.QTableWidget(self.centralwidget) #for block diagram
        # self.tableWidget_4.setGeometry(QtCore.QRect(940, 370, 381, 237))
        # self.tableWidget_4.setRowCount(5)
        # self.tableWidget_4.setColumnCount(2000)
        # self.tableWidget_4.setObjectName("tableWidget_4")
        # self.tableWidget_4.setColumnWidth(0,40)
        # for i in range(self.tableWidget_4.columnCount()):
        #     self.tableWidget_4.setColumnWidth(i, 40)
        self.label_8 = QtWidgets.QLabel(self.centralwidget)
        self.label_8.setGeometry(QtCore.QRect(960, 340, 338, 30))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_8.setFont(font)
        self.label_8.setObjectName("label_8")
        self.textBrowser_3 = QtWidgets.QTextBrowser(self.centralwidget) #text box of assemble log
        self.textBrowser_3.setGeometry(QtCore.QRect(890, 160, 181, 161))
        self.textBrowser_3.setObjectName("textBrowser_3")
        self.label_9 = QtWidgets.QLabel(self.centralwidget)
        self.textBrowser_3.setStyleSheet("background-color : silver")
        self.label_9.setGeometry(QtCore.QRect(920, 130, 138, 30))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_9.setFont(font)
        self.label_9.setObjectName("label_9")
        self.textBrowser_4 = QtWidgets.QTextBrowser(self.centralwidget) #text box of assemble log
        self.textBrowser_4.setGeometry(QtCore.QRect(1085, 160, 191, 161))
        self.textBrowser_4.setObjectName("textBrowser_4")
        self.textBrowser_4.setStyleSheet("background-color : silver")
        self.label_10 = QtWidgets.QLabel(self.centralwidget)
        self.label_10.setGeometry(QtCore.QRect(1120, 130, 138, 30))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_10.setFont(font)
        self.label_10.setObjectName("label_10")
        self.tableWidget_5 = QtWidgets.QTableWidget(self.centralwidget) #for all stats
        self.tableWidget_5.setGeometry(QtCore.QRect(895, 360, 381, 500))
        self.tableWidget_5.setStyleSheet("background-color : silver")
        self.tableWidget_5.setRowCount(12)
        self.tableWidget_5.setColumnCount(1)
        self.tableWidget_5.setObjectName("tableWidget_4")
        self.tableWidget_5.setColumnWidth(0,320)
        self.label_11 = QtWidgets.QLabel(self.centralwidget)
        self.label_11.setGeometry(QtCore.QRect(1040, 365, 142, 30))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_11.setFont(font)
        self.label_11.setObjectName("label_11")
        self.line_3 = QtWidgets.QFrame(self.centralwidget)
        self.line_3.setGeometry(QtCore.QRect(1290, 60, 2, 861))
        self.line_3.setFrameShape(QtWidgets.QFrame.VLine)
        self.line_3.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_3.setObjectName("line_3")
        self.line_3.setStyleSheet("background-color: black;")

        #check box work
        self.radioBox_4 = QRadioButton(self.centralwidget)
        self.radioBox_4.setObjectName(u"radioBox_4")
        self.radioBox_4.setGeometry(QRect(1130, 63, 62, 17))
        self.radioBox_4.setFont(QtGui.QFont("sanserif", 11))
        self.radioBox_2 = QRadioButton(self.centralwidget)
        self.radioBox_2.setObjectName(u"radioBox_2")
        self.radioBox_2.setGeometry(QRect(1180, 100, 62, 17))
        self.radioBox_2.setFont(QtGui.QFont("sanserif", 11))
        self.radioBox_3 = QRadioButton(self.centralwidget)
        self.radioBox_3.setObjectName(u"radioBox_3")
        self.radioBox_3.setGeometry(QRect(890, 100, 62, 17))
        self.radioBox_3.setFont(QtGui.QFont("sanserif", 11))
        self.radioBox_5 = QRadioButton(self.centralwidget)
        self.radioBox_5.setObjectName(u"radioBox_5")
        self.radioBox_5.setGeometry(QRect(1080, 100, 62, 17))
        self.radioBox_5.setFont(QtGui.QFont("sanserif", 11))

        self.radioBox_5.hide()
        self.radioBox_2.hide()
        self.radioBox_3.hide()
        self.checkBox = QPushButton(self.centralwidget)
        self.checkBox.setObjectName(u"checkBox")
        self.checkBox.setGeometry(QRect(960, 60, 82, 17))
        self.checkBox.setFont(QtGui.QFont("sanserif", 11))
        self.checkBox.setStyleSheet("background-color : red")
        
        #cache part
        self.label_12 = QtWidgets.QLabel(self.centralwidget) #cache memory label
        self.label_12.setGeometry(QtCore.QRect(1390, 30, 191, 30))
        font = QtGui.QFont()
        font.setPointSize(16)
        self.label_12.setFont(font)
        self.label_12.setObjectName("label_12")

        # self.label_13 = QtWidgets.QLabel(self.centralwidget)#cache size
        # self.label_13.setGeometry(QtCore.QRect(1320, 70, 101, 20))
        # font = QtGui.QFont()
        # font.setPointSize(11)
        # self.label_13.setFont(font)
        # self.label_13.setObjectName("label_10")
        # self.textBrowser_13 = QtWidgets.QTextBrowser(self.centralwidget) #text box of assemble log
        # self.textBrowser_13.setGeometry(QtCore.QRect(1435, 70, 101, 20))
        # self.textBrowser_13.setObjectName("textBrowser_13")
        # self.textBrowser_13.setStyleSheet("background-color : silver")

        # self.label_14 = QtWidgets.QLabel(self.centralwidget)#block size
        # self.label_14.setGeometry(QtCore.QRect(1320, 110, 101, 20))
        # font = QtGui.QFont()
        # font.setPointSize(11)
        # self.label_14.setFont(font)
        # self.label_14.setObjectName("label_10")
        # self.textBrowser_14 = QtWidgets.QTextBrowser(self.centralwidget) #text box of assemble log
        # self.textBrowser_14.setGeometry(QtCore.QRect(1435, 110, 191, 20))
        # self.textBrowser_14.setObjectName("textBrowser_14")
        # self.textBrowser_14.setStyleSheet("background-color : silver")

        # self.label_15 = QtWidgets.QLabel(self.centralwidget) #ways of SA
        # self.label_15.setGeometry(QtCore.QRect(1320, 150, 101, 20))
        # font = QtGui.QFont()
        # font.setPointSize(11)
        # self.label_15.setFont(font)
        # self.label_15.setObjectName("label_10")
        # self.textBrowser_15 = QtWidgets.QTextBrowser(self.centralwidget) #text box of assemble log
        # self.textBrowser_15.setGeometry(QtCore.QRect(1435, 150, 191, 20))
        # self.textBrowser_15.setObjectName("textBrowser_15")
        # self.textBrowser_15.setStyleSheet("background-color : silver")

        # self.label_18 = QtWidgets.QLabel(self.centralwidget) #ways of SA
        # self.label_18.setGeometry(QtCore.QRect(1320, 190, 101, 20))
        # font = QtGui.QFont()
        # font.setPointSize(11)
        # self.label_18.setFont(font)
        # self.label_18.setObjectName("label_10")
        # self.textBrowser_18 = QtWidgets.QTextBrowser(self.centralwidget) #text box of assemble log
        # self.textBrowser_18.setGeometry(QtCore.QRect(1435, 190, 101, 20))
        # self.textBrowser_18.setObjectName("textBrowser_18")
        # self.textBrowser_18.setStyleSheet("background-color : silver")
        
        self.label_20 = QtWidgets.QLabel(self.centralwidget) #ways of SA
        self.label_20.setGeometry(QtCore.QRect(1340, 60, 101, 20))
        font = QtGui.QFont()
        font.setPointSize(11)
        self.label_20.setFont(font)
        self.label_20.setObjectName("label_20")
        self.textBrowser_20 = QtWidgets.QTextBrowser(self.centralwidget) #text box of assemble log
        self.textBrowser_20.setGeometry(QtCore.QRect(1305, 90, 241, 161))
        self.textBrowser_20.setObjectName("textBrowser_20")
        # self.textBrowser_20.setStyleSheet("background-color : silver")

        self.label_21 = QtWidgets.QLabel(self.centralwidget) #ways of SA
        self.label_21.setGeometry(QtCore.QRect(1580, 60, 101, 20))
        font = QtGui.QFont()
        font.setPointSize(11)
        self.label_21.setFont(font)
        self.label_21.setObjectName("label_21")
        self.textBrowser_21 = QtWidgets.QTextBrowser(self.centralwidget) #text box of assemble log
        self.textBrowser_21.setGeometry(QtCore.QRect(1575, 90, 241, 161))
        self.textBrowser_21.setObjectName("textBrowser_21")
        # self.textBrowser_21.setStyleSheet("background-color : silver")

        self.tableWidget_8 = QtWidgets.QTableWidget(self.centralwidget) #for all stats
        self.tableWidget_8.setGeometry(QtCore.QRect(1355, 265, 301, 235))
        # self.tableWidget_8.setStyleSheet("background-color : silver")
        self.tableWidget_8.setRowCount(5)
        self.tableWidget_8.setColumnCount(1)
        self.tableWidget_8.setObjectName("tableWidget_8")
        self.tableWidget_8.setColumnWidth(0,320)
        self.label_19 = QtWidgets.QLabel(self.centralwidget)
        self.label_19.setGeometry(QtCore.QRect(1455, 265, 142, 30))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_19.setFont(font)
        self.label_19.setObjectName("label_19")
        
        # self.tabWidget = QTabWidget(self.centralwidget)
        # self.tabWidget.setObjectName(u"tabWidget")
        # self.tabWidget.setGeometry(QRect(1300, 500, 880, 431))
        # self.tab = QWidget()
        # self.tab.setObjectName(u"tab")
        # self.label_16 = QtWidgets.QLabel(self.tab) #data cache
        # self.label_16.setGeometry(QtCore.QRect(220, 10, 111, 20))
        # font = QtGui.QFont()
        # font.setPointSize(13)
        # self.label_16.setFont(font)
        # self.label_16.setObjectName("label_10")

        # self.tableWidget_7 = QtWidgets.QTableWidget(self.tab)
        # self.tableWidget_7.setGeometry(QtCore.QRect(10, 40, 536, 521))
        # self.tableWidget_7.setRowCount(30)
        # self.tableWidget_7.setColumnCount(20)
        # self.tableWidget_7.setObjectName("tableWidget_7")
        # for i in range(200):
        #     self.tableWidget_7.setColumnWidth(i,150)
        # self.tabWidget.addTab(self.tab, "")

        # self.tab_2 = QWidget()
        # self.tab_2.setObjectName(u"tab_2")
        # self.tableWidget_6 = QtWidgets.QTableWidget(self.tab_2)
        # self.tableWidget_6.setGeometry(QtCore.QRect(10, 40, 536, 521))
        # self.tableWidget_6.setRowCount(30)
        # self.tableWidget_6.setColumnCount(20)
        # self.tableWidget_6.setObjectName("tableWidget_6")
        # for i in range(200):
        #     self.tableWidget_6.setColumnWidth(i,150)

        # self.label_17 = QtWidgets.QLabel(self.tab_2)#instruction cache
        # self.label_17.setGeometry(QtCore.QRect(220, 10, 101, 20))
        # font = QtGui.QFont()
        # font.setPointSize(13)
        # self.label_17.setFont(font)
        # self.label_17.setObjectName("label_10")
        # self.tabWidget.addTab(self.tab_2, "")

        self.retranslateUi(MainWindow)
            
    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "RISC_Window"))
        self.label.setText(_translate("MainWindow", "RISC-V Simulator"))
        self.pushButton.setText(_translate("MainWindow", "README"))
        self.pushButton_2.setText(_translate("MainWindow", "Test Case"))
        self.pushButton_3.setText(_translate("MainWindow", "RUN"))
        self.pushButton_4.setText(_translate("MainWindow", "Output Log"))
        self.toggleButton.setText(_translate("MainWindow", "Change Theme"))
        self.label_3.setText(_translate("MainWindow", "Register View"))
        self.label_4.setText(_translate("MainWindow", "Memory View"))
        self.label_2.setText(_translate("MainWindow", "Output"))
        self.pushButton_5.setText(_translate("MainWindow", "STEP"))
        self.pushButton_6.setText(_translate("MainWindow", "ASSEMBLE"))
        self.label_5.setText(_translate("MainWindow", "PC :"))
        self.label_6.setText(_translate("MainWindow", "IR :"))
        # self.label_8.setText(_translate("MainWindow", "Block diagram of 5 pipeline stages"))
        self.label_9.setText(_translate("MainWindow", "D_H Cycles"))
        self.label_10.setText(_translate("MainWindow", "C_H Cycles"))
        self.label_11.setText(_translate("MainWindow", "ALL STATS"))

        #cache part
        # self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab),_translate("MainWindow", u"Data Cache", None))
        # self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_2),_translate("MainWindow", u"Instruction Cache", None))
        self.label_12.setText(_translate("MainWindow", "Cache Memory"))
        # self.label_19.setText(_translate("MainWindow", "Instruction"))
        # self.label_13.setText(_translate("MainWindow", "Cache Size"))
        # self.label_14.setText(_translate("MainWindow", "Block Size"))
        # self.label_15.setText(_translate("MainWindow", "Ways for SA"))
        # self.label_18.setText(_translate("MainWindow", "Associativity"))
        # self.label_16.setText(_translate("MainWindow", "Data Cache"))
        # self.label_17.setText(_translate("MainWindow", "Instruction Cache"))
        self.label_19.setText(_translate("MainWindow", "Cache Stats"))
        self.label_20.setText(_translate("MainWindow", "n-bit add"))
        self.label_21.setText(_translate("MainWindow", "Victims"))
        self.checkBox.setText(_translate("MainWindow", u"Pipelined Execution", None))
        self.radioBox_3.setText(_translate("MainWindow", u"Data Forwarding", None))
        self.radioBox_2.setText(_translate("MainWindow", u"Stalling", None))
        self.radioBox_4.setText(_translate("MainWindow", u"Single Cycle", None))
        self.radioBox_5.setText(_translate("MainWindow", u"Cache", None))

        self.label.adjustSize()
        self.label_2.adjustSize()
        self.label_3.adjustSize()
        self.label_4.adjustSize()
        self.label_5.adjustSize()
        self.label_6.adjustSize()
        # self.label_15.adjustSize()
        # self.label_14.adjustSize()
        # self.label_13.adjustSize()
        # self.label_17.adjustSize()
        # self.label_16.adjustSize()
        self.label_19.adjustSize()
        self.checkBox.adjustSize()
        self.radioBox_2.adjustSize()
        self.radioBox_3.adjustSize()
        self.radioBox_4.adjustSize()
        self.radioBox_5.adjustSize()
        self.radioBox_4.setChecked(True)
        self.pushButton_2.clicked.connect(lambda: self.opendata())
        self.pushButton.clicked.connect(lambda: self.guihelp())
        self.pushButton_4.clicked.connect(lambda: self.outputlog())
        self.toggleButton.clicked.connect(lambda: self.changeTheme())
        self.pushButton_6.clicked.connect(lambda: self.assembly())
        self.pushButton_5.clicked.connect(lambda: self.step())
        self.pushButton_3.clicked.connect(lambda: self.run())
        self.checkBox.clicked.connect(lambda: self.pipelining())
    
    def pipelining(self):
        if(self.pipeline):
            self.pipeline=0
            self.radioBox_2.setHidden(True)
            self.radioBox_3.setHidden(True)
            self.radioBox_5.setHidden(True)
            self.checkBox.setStyleSheet("background-color : red")
        else:
            self.radioBox_2.setHidden(False)
            self.radioBox_3.setHidden(False)
            self.radioBox_5.setHidden(False)
            self.pipeline=1
            self.checkBox.setStyleSheet("background-color : green")

    def assembly(self):
        string=os.path.abspath(os.getcwd())
        string2 = string  
        if(self.radioBox_3.isChecked()):  
            if (os.name == "posix"):  #If the OS used is Ubuntu 
                string +="\/main.cpp data_forwarding.cpp -I ../include"
                string = "g++ " + string
                os.system(string)
                string2 += "\/./a.out"
                os.system(string2)
            elif (os.name == "nt"):   #If the OS used is Windows
                string +="\main.cpp data_forwarding.cpp -I ../include"
                string = "g++ " + string
                os.system(string)
                string2 += "./a.exe"
                os.system(string2)
        elif(self.radioBox_2.isChecked()):
            if (os.name == "posix"):  #If the OS used is Ubuntu 
                string +="\/main.cpp StalledPipeline.cpp -I ../include"
                string = "g++ " + string
                os.system(string)
                string2 += "\/./a.out"
                os.system(string2)
            elif (os.name == "nt"):   #If the OS used is Windows
                string +="\main.cpp StalledPipeline.cpp -I ../include"
                string = "g++ " + string
                os.system(string)
                string2 += "./a.exe"
                os.system(string2)  
        elif(self.radioBox_5.isChecked()):
            if (os.name == "posix"):  #If the OS used is Ubuntu 
                string +="\/main.cpp myRISCVSim.cpp -I ../include"
                string = "g++ " + string
                os.system(string)
                string2 += "\/./a.out"
                os.system(string2)
            elif (os.name == "nt"):   #If the OS used is Windows
                string +="\main.cpp myRISCVSim.cpp -I ../include"
                string = "g++ " + string
                os.system(string)
                string2 += "./a.exe"
                os.system(string2)  
        else:
            if (os.name == "posix"):  #If the OS used is Ubuntu 
                string +="\/main.cpp SingleCycle.cpp -I ../include"
                string = "g++ " + string
                os.system(string)
                string2 += "\/./a.out"
                os.system(string2)
            elif (os.name == "nt"):   #If the OS used is Windows
                string +="\main.cpp SingleCycle.cpp -I ../include"
                string = "g++ " + string
                os.system(string)
                string2 += "./a.exe"
                os.system(string2)
        self.textBrowser_2.append("Ready to run!!!!")
    
    def guihelp(self):
        webbrowser.open('https://github.com/anushthaPrakash/CS_204_RISCV')

    def run(self):
        self.textBrowser.clear()
        self.tableWidget_5.clear()
        self.textBrowser_3.clear()
        self.textBrowser_4.clear()
        data=open('output_log.mem','r')
        for i in data:
            self.textBrowser.append(i)

        data2 = open('register.mem','r')
        i = 0
        for item in data2:
            temp2=str(item)
            self.tableWidget.setItem(i,0,QtWidgets.QTableWidgetItem(temp2))
            i += 1

        i =0
        data3 = open('D_Memory.mem','r')
        for item2 in data3:
            temp2=str(item2)
            self.tableWidget_2.setItem(i,0,QtWidgets.QTableWidgetItem(temp2))
            i += 1
        i =0
        if(self.radioBox_2.isChecked()):
            data4 = open('CountData.mem','r')
            for item2 in data4:
                temp2=str(item2)
                self.tableWidget_5.setItem(i,0,QtWidgets.QTableWidgetItem(temp2))
                i += 1
            i = 0
            data5 = open('DH_Cycle.mem','r')
            self.textBrowser_3.clear()
            for i in data5:
                self.textBrowser_3.append(i)  
            data5 = open('CH_Cycle.mem','r')
            self.textBrowser_4.clear()
            for i in data5:
                self.textBrowser_4.append(i)  

        elif(self.radioBox_3.isChecked()):
            data4 = open('CountData.mem','r')
            for item2 in data4:
                temp2=str(item2)
                self.tableWidget_5.setItem(i,0,QtWidgets.QTableWidgetItem(temp2))
                i += 1
            i = 0
            data5 = open('DH_Cycle.mem','r')
            self.textBrowser_3.clear()
            for i in data5:
                self.textBrowser_3.append(i)  
            data6 = open('CH_Cycle.mem','r')
            self.textBrowser_4.clear()
            for i in data6:
                self.textBrowser_4.append(i)  
        
        elif(self.radioBox_5.isChecked()):
            data4 = open('ICacheData.mem','r')
            for item2 in data4:
                temp2=str(item2)
                self.tableWidget_8.setItem(i,0,QtWidgets.QTableWidgetItem(temp2))
                i += 1
            i = 0

            data5 = open('FetchTotalData.mem','r')
            self.textBrowser_20.clear()
            for i in data5:
                self.textBrowser_20.append(i)  

            data6 = open('FetchData.mem','r')
            self.textBrowser_21.clear()
            for i in data6:
                self.textBrowser_21.append(i)

            # data7 = open('ICACHE.mem','r')
            # for item2 in data7:
            #     temp2=str(item2)
            #     self.tableWidget_6.setItem(i,0,QtWidgets.QTableWidgetItem(temp2))
            #     i += 1
            # i = 0  
            # data8 = open('DCACHE.mem','r')
            # for item2 in data8:
            #     temp2=str(item2)
            #     self.tableWidget_7.setItem(i,0,QtWidgets.QTableWidgetItem(temp2))
            #     i += 1
            # i = 0  

            
    def opendata(self):
        string=""
        string +="inp.py"
        string = "python " + string
        os.system(string)
        try:
            string=os.path.abspath(os.getcwd())
            string+="\\test_case.mem"
            os.startfile(string)
        except:
            opener = "open" if sys.platform == "darwin" else "xdg-open"
            subprocess.call([opener, "test_case.mem"])
            
    def outputlog(self):
        try:
            string=os.path.abspath(os.getcwd())
            string+="\output_log.mem"
            os.startfile(string)
        except:
            opener = "open" if sys.platform == "darwin" else "xdg-open"
            subprocess.call([opener, "output_log.mem"])

    def changeTheme(self):
        if self.currentTheme == "L":
            self.tableWidget_5.setStyleSheet("background-color : black")
            # self.tableWidget_6.setStyleSheet("background-color : black")
            # self.tableWidget_7.setStyleSheet("background-color : black")
            self.tableWidget_8.setStyleSheet("background-color : black")
            self.tableWidget_2.setStyleSheet("background-color : black")
            self.tableWidget.setStyleSheet("background-color : black")
            self.textBrowser_4.setStyleSheet("background-color : black")
            self.textBrowser_3.setStyleSheet("background-color : black")
            self.textBrowser_2.setStyleSheet("background-color : black")
            self.textBrowser.setStyleSheet("background-color : black")
            self.textBrowser_20.setStyleSheet("background-color : black")
            self.textBrowser_21.setStyleSheet("background-color : black")
            self.currentTheme = "D"
            self.centralwidget.setStyleSheet(qdarkstyle.load_stylesheet(qt_api='pyqt5', palette=DarkPalette))
        elif self.currentTheme == "D":
            self.tableWidget_5.setStyleSheet("background-color : silver")
            # self.tableWidget_6.setStyleSheet("background-color : white")
            # self.tableWidget_7.setStyleSheet("background-color : white")
            self.tableWidget_8.setStyleSheet("background-color : white")
            self.textBrowser_4.setStyleSheet("background-color : silver")
            self.textBrowser_3.setStyleSheet("background-color : silver")
            self.textBrowser_2.setStyleSheet("background-color : white")
            self.textBrowser.setStyleSheet("background-color : white")
            self.tableWidget_2.setStyleSheet("background-color : white")
            self.tableWidget.setStyleSheet("background-color : white")
            self.textBrowser_20.setStyleSheet("background-color : white")
            self.textBrowser_21.setStyleSheet("background-color : white")
            self.currentTheme = "L"
            self.centralwidget.setStyleSheet("")

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
