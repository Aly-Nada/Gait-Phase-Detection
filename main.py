from kivy.app import App
from kivy.uix.label import Label
from kivy.clock import Clock
from kivy.graphics import Color, Ellipse
from kivy.core.window import Window
from kivy.graphics.svg import Svg
from kivy.uix.floatlayout import FloatLayout
import requests
import csv


class MainWidgetClass(Label):
    def update(self, *args):
        self.color = [105, 106, 188, 1]
        self.pos = (0, -250)
        self.font_size = '50dp'

        with self.canvas:
            r = requests.get('http://192.168.1.15/all')
            with open('csv_file.csv', 'a') as f:
                writer = csv.writer(f)
                writer.writerow([r.json()["data1"], r.json()["data2"], r.json()["data3"], r.json()["data4"], r.json()["data5"]])

            col1 = r.json()["data1"]/4096
            col2 = r.json()["data2"]/4096 # Toe
            col3 = r.json()["data3"]/4096 # Heel
            col4 = r.json()["data4"]/4096 # Right
            col5 = r.json()["data5"]/4096 # Left
            Color(col1, col1, col1)
            Ellipse(pos=(230, 530), size=(30, 30))
            Color(col5, col5, col5)
            Ellipse(pos=(180, 430), size=(30, 30))
            Color(col4, col4, col4)
            Ellipse(pos=(280, 450), size=(30, 30))
            Color(col3, col3, col3)
            Ellipse(pos=(240, 200), size=(30, 30))
        if col2 < 0.5 and col3 < 0.5 and col4 < 0.5 and col5 < 0.5:
            self.text = "MID SWING"
        if col2 < 0.5 and col3 > 0.5 and col4 < 0.5 and col5 < 0.5:
            self.text = "HEEL STRIKE"
        if col2 > 0.5 and col3 < 0.5 and col4 > 0.5 and col5 > 0.5:
            self.text = "HEEL OFF"
        if col2 > 0.5 and col3 < 0.5 and col4 < 0.5 and col5 < 0.5:
            self.text = "TOE OFF"


class SvgWidget(Label):
    def __init__(self, filename):
        self.pos = (200, -200)
        super(SvgWidget, self).__init__()
        with self.canvas:
            svg = Svg(filename)
        self.size = svg.width, svg.height
        self.anchor_x = -100


class ESP32FootApp(App):
    def build(self):
        self.root = FloatLayout()

        Window.size = (500, 700)
        Window.clearcolor = (0.2, 0.2, 0.2, 1)

        filename = "foot.svg"
        svg = SvgWidget(filename)
        self.root.add_widget(svg)
        svg.scale = 1.1

        mainWidget = MainWidgetClass()
        Clock.schedule_interval(mainWidget.update, 0.05)
        self.root.add_widget(mainWidget)


if __name__ == "__main__":
    ESP32FootApp().run()
