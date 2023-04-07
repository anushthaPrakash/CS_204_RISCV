from PyQt5 import *
from PyQt5 import QtCore, QtGui, QtWidgets
import os,sys,subprocess
from PyQt5.QtWidgets import QApplication, QDialog, QLabel, QGroupBox, QVBoxLayout, QHBoxLayout, QCheckBox, QRadioButton
from PyQt5.QtWidgets import *
from PyQt5.QtCore import QRect
import qdarkstyle
from qdarkstyle.dark.palette import DarkPalette
import webbrowser
import inp

class Ui_MainWindow(object):
    pipeline = 0
    def setupUi(self, MainWindow):
        self.currentTheme = "L"
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1820, 1021)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(170, 0, 361, 41))
        font = QtGui.QFont()
        font.setPointSize(24)
        self.label.setFont(font)
        self.label.setObjectName("label")
        self.pushButton = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton.setGeometry(QtCore.QRect(0, 0, 89, 25))
        self.pushButton.setObjectName("pushButton")
        self.pushButton_2 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_2.setGeometry(QtCore.QRect(370, 70, 131, 31))
        self.pushButton_2.setObjectName("pushButton_2")
        self.pushButton_3 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_3.setGeometry(QtCore.QRect(445, 230, 131, 31))
        self.pushButton_3.setObjectName("pushButton_3")
        self.pushButton_4 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_4.setGeometry(QtCore.QRect(90, 750, 89, 25))
        self.pushButton_4.setObjectName("pushButton_4")
        self.toggleButton = QtWidgets.QPushButton(self.centralwidget)
        self.toggleButton.setGeometry(QtCore.QRect(450, 775, 110, 30))
        self.toggleButton.setObjectName("toggleButton")

        font = QtGui.QFont()
        self.tableWidget = QtWidgets.QTableWidget(self.centralwidget)  #for register data table 
        self.tableWidget.setGeometry(QtCore.QRect(310, 430, 256, 331)) #last index is to increase height 
        self.tableWidget.setRowCount(32)
        self.tableWidget.setColumnCount(1)
        self.tableWidget.setObjectName("tableWidget")

        self.line_3 = QtWidgets.QFrame(self.centralwidget)
        self.line_3.setGeometry(QtCore.QRect(580, 390, 20, 441))
        self.line_3.setFrameShape(QtWidgets.QFrame.VLine)
        self.line_3.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_3.setObjectName("line_3")
        self.label_3 = QtWidgets.QLabel(self.centralwidget)
        self.label_3.setGeometry(QtCore.QRect(320, 390, 211, 21))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_3.setFont(font)
        self.label_3.setObjectName("label_3")
        self.label_4 = QtWidgets.QLabel(self.centralwidget)
        self.label_4.setGeometry(QtCore.QRect(700, 40, 211, 20))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_4.setFont(font)
        self.label_4.setObjectName("label_4")
        self.label_2 = QtWidgets.QLabel(self.centralwidget)
        self.label_2.setGeometry(QtCore.QRect(90, 50, 71, 17))
        self.label_2.setObjectName("label_2")
        self.textBrowser = QtWidgets.QTextBrowser(self.centralwidget) #text box of output log
        self.textBrowser.setGeometry(QtCore.QRect(10, 80, 271, 661))
        self.textBrowser.setObjectName("textBrowser")
        self.tableWidget_2 = QtWidgets.QTableWidget(self.centralwidget)
        self.tableWidget_2.setGeometry(QtCore.QRect(620, 70, 291, 741))
        self.tableWidget_2.setObjectName("tableWidget_2")
        self.tableWidget_2.setColumnCount(1)
        self.tableWidget_2.setRowCount(250000)
        
        self.pushButton_5 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_5.setGeometry(QtCore.QRect(305, 230, 131, 31))
        self.pushButton_5.setObjectName("pushButton_5")
        self.pushButton_6 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_6.setGeometry(QtCore.QRect(385, 120, 101, 31))
        self.pushButton_6.setObjectName("pushButton_6")
        self.textBrowser_2 = QtWidgets.QTextBrowser(self.centralwidget) #text box of assemble log
        self.textBrowser_2.setGeometry(QtCore.QRect(305, 160, 271, 61))
        self.textBrowser_2.setObjectName("textBrowser_2")

        self.label_5 = QtWidgets.QLabel(self.centralwidget)
        self.label_5.setGeometry(QtCore.QRect(320, 280, 81, 31))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_5.setFont(font)
        self.label_5.setObjectName("label_5")
        self.label_6 = QtWidgets.QLabel(self.centralwidget)
        self.label_6.setGeometry(QtCore.QRect(320, 330, 67, 17))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_6.setFont(font)
        self.label_6.setObjectName("label_6")

        self.line_4 = QtWidgets.QFrame(self.centralwidget)
        self.line_4.setGeometry(QtCore.QRect(580, 60, 20, 381))
        self.line_4.setFrameShape(QtWidgets.QFrame.VLine)
        self.line_4.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_4.setObjectName("line_4")
        self.label_18 = QtWidgets.QLabel(self.centralwidget)
        self.label_18.setGeometry(QtCore.QRect(650, 80, 131, 20))
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
        self.tableWidget_4 = QtWidgets.QTableWidget(self.centralwidget) #for block diagram
        self.tableWidget_4.setGeometry(QtCore.QRect(940, 370, 381, 237))
        self.tableWidget_4.setRowCount(5)
        self.tableWidget_4.setColumnCount(2000)
        self.tableWidget_4.setObjectName("tableWidget_4")
        self.tableWidget_4.setColumnWidth(0,40)
        for i in range(self.tableWidget_4.columnCount()):
            self.tableWidget_4.setColumnWidth(i, 40)
        self.label_8 = QtWidgets.QLabel(self.centralwidget)
        self.label_8.setGeometry(QtCore.QRect(960, 340, 338, 30))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_8.setFont(font)
        self.label_8.setObjectName("label_8")
        self.textBrowser_3 = QtWidgets.QTextBrowser(self.centralwidget) #text box of assemble log
        self.textBrowser_3.setGeometry(QtCore.QRect(935, 160, 191, 161))
        self.textBrowser_3.setObjectName("textBrowser_3")
        self.label_9 = QtWidgets.QLabel(self.centralwidget)
        self.label_9.setGeometry(QtCore.QRect(960, 130, 138, 30))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_9.setFont(font)
        self.label_9.setObjectName("label_9")
        self.textBrowser_4 = QtWidgets.QTextBrowser(self.centralwidget) #text box of assemble log
        self.textBrowser_4.setGeometry(QtCore.QRect(1135, 160, 191, 161))
        self.textBrowser_4.setObjectName("textBrowser_4")
        self.label_10 = QtWidgets.QLabel(self.centralwidget)
        self.label_10.setGeometry(QtCore.QRect(1160, 130, 138, 30))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_10.setFont(font)
        self.label_10.setObjectName("label_10")
        self.tableWidget_5 = QtWidgets.QTableWidget(self.centralwidget) #for all stats
        self.tableWidget_5.setGeometry(QtCore.QRect(940, 640, 381, 237))
        self.tableWidget_5.setRowCount(12)
        self.tableWidget_5.setColumnCount(1)
        self.tableWidget_5.setObjectName("tableWidget_4")
        self.tableWidget_5.setColumnWidth(0,320)
        self.label_11 = QtWidgets.QLabel(self.centralwidget)
        self.label_11.setGeometry(QtCore.QRect(1080, 605, 142, 30))
        font = QtGui.QFont()
        font.setPointSize(13)
        self.label_11.setFont(font)
        self.label_11.setObjectName("label_10")

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
        self.radioBox_3.setGeometry(QRect(960, 100, 62, 17))
        self.radioBox_3.setFont(QtGui.QFont("sanserif", 11))
        self.radioBox_2.hide()
        self.radioBox_3.hide()
        self.checkBox = QPushButton(self.centralwidget)
        self.checkBox.setObjectName(u"checkBox")
        self.checkBox.setGeometry(QRect(960, 60, 82, 17))
        self.checkBox.setFont(QtGui.QFont("sanserif", 11))
        self.checkBox.setStyleSheet("background-color : red")
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
        self.label_8.setText(_translate("MainWindow", "Block diagram of 5 pipeline stages"))
        self.label_9.setText(_translate("MainWindow", "D_H Cycles"))
        self.label_10.setText(_translate("MainWindow", "C_H Cycles"))
        self.label_11.setText(_translate("MainWindow", "ALL STATS"))
        self.checkBox.setText(_translate("MainWindow", u"Pipelined Execution", None))
        self.radioBox_3.setText(_translate("MainWindow", u"Data Forwarding", None))
        self.radioBox_2.setText(_translate("MainWindow", u"Stalling", None))
        self.radioBox_4.setText(_translate("MainWindow", u"Single Cycle", None))
        self.label.adjustSize()
        self.label_2.adjustSize()
        self.label_3.adjustSize()
        self.label_4.adjustSize()
        self.label_5.adjustSize()
        self.label_6.adjustSize()
        self.checkBox.adjustSize()
        self.radioBox_2.adjustSize()
        self.radioBox_3.adjustSize()
        self.radioBox_4.adjustSize()
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
            self.checkBox.setStyleSheet("background-color : red")
        else:
            self.radioBox_2.setHidden(False)
            self.radioBox_3.setHidden(False)
            self.radioBox_4.setHidden(False)
            self.pipeline=1
            self.checkBox.setStyleSheet("background-color : green")

    def assembly(self):
        string=os.path.abspath(os.getcwd())
        string2 = string  
        if(self.radioBox_2.isChecked()):
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
        elif(self.radioBox_3.isChecked()):
            if (os.name == "posix"):  #If the OS used is Ubuntu 
                string +="\/main.cpp simple_Stalled_Pipeline.cpp -I ../include"
                string = "g++ " + string
                os.system(string)
                string2 += "\/./a.out"
                os.system(string2)
            elif (os.name == "nt"):   #If the OS used is Windows
                string +="\main.cpp simple_Stalled_Pipeline.cpp -I ../include"
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
            data5 = open('CountData.mem','r')
            for i in data5:
                self.textBrowser_3.append(i)  
            data5 = open('CountData.mem','r')
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
            for i in data5:
                self.textBrowser_3.append(i)  
            data5 = open('CH_Cycle.mem','r')
            for i in data5:
                self.textBrowser_4.append(i)  
            

    def changeTheme(self):
        if self.currentTheme == "L":
            self.currentTheme = "D"
            self.centralwidget.setStyleSheet(qdarkstyle.load_stylesheet(qt_api='pyqt5', palette=DarkPalette))
        elif self.currentTheme == "D":
            self.currentTheme = "L"
            self.centralwidget.setStyleSheet("")

    def opendata(self):
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
if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
