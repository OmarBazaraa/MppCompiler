import math, sys
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import *
from PyQt5.QtWidgets import QApplication, QWidget, QInputDialog, QLineEdit, QFileDialog
from PyQt5.QtGui import *
from PyQt5 import QtWidgets, QtCore
from PyQt5.QtWidgets import *
from PyQt5.QtCore import Qt, QTimer

class Overlay(QWidget):

    def __init__(self, parent = None):
    
        QWidget.__init__(self, parent)
        palette = QPalette(self.palette())
        palette.setColor(palette.Background, Qt.transparent)
        self.setPalette(palette)
    
    def paintEvent(self, event):
    
        painter = QPainter()
        painter.begin(self)
        painter.setRenderHint(QPainter.Antialiasing)
        painter.fillRect(event.rect(), QBrush(QColor(255, 255, 255, 200)))
        painter.setPen(QPen(Qt.NoPen))

        for i in range(6):
            if (math.ceil(self.delayedCounter / 5) % 6) == i:
                painter.setBrush(QBrush(QColor(255, 100, 100)))
            else:
                painter.setBrush(QBrush(QColor(127, 127, 127)))
                
            painter.drawEllipse(
                self.width()/2 + 30 * math.cos(2 * math.pi * i / 6.0) - 10,
                self.height()/2 + 30 * math.sin(2 * math.pi * i / 6.0) - 10,
                20, 20)
        
        painter.end()
    
    def showEvent(self, event):
    
        self.timer = self.startTimer(50)
        self.counter = 0
        self.delayedCounter = 0
    
    def timerEvent(self, event):
    
        self.counter += 1
        self.update()

        if (self.counter % 2 == 0):
            self.delayedCounter += 1
            
        
