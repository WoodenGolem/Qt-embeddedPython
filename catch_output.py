import sys
from time import sleep


class CatchOutErr:
    def __init__(self):
        self.value = ''
        self.currentLine = ''
        self.lineAvailable = False
        
    def write(self, txt):
        self.value += txt
        self.lineAvailable = True

    def clear(self):
        self.value = ''
        self.lineAvailable = False

    def update(self):
        self.currentLine = self.value.split('\n')[0]
        self.value = self.value.replace(self.currentLine + '\n', '', 1)
        if (self.value == ''):
            self.lineAvailable = False


catchOutErr = CatchOutErr()
sys.stdout = catchOutErr
sys.stderr = catchOutErr
