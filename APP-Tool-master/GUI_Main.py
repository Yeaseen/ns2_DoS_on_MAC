#!/usr/bin/python
# GUI for the tracefile analysis tool
import os, subprocess
from gi.repository import Gtk
import matplotlib.pyplot as plt

UI_INFO = """
<ui>
  <menubar name='MenuBar'>
    <menu action='FileMenu'>
      <menuitem action='Tracefile' />
      <separator />
      <menuitem action='FileQuit' />
    </menu>
    <menu action='HelpMenu'>
      <menuitem action='Documentation' />
      <menuitem action='Credits' />
      <menuitem action='Help' />
    </menu>
  </menubar>
</ui>
"""

class output_dialog(Gtk.Dialog):

    def __init__(self, parent, result):
        Gtk.Dialog.__init__(self, "Output", parent, 0,
            ("Export to file", 66, Gtk.STOCK_OK, Gtk.ResponseType.OK))

        self.set_default_size(550, 500)

        self.vbox_op = Gtk.Box(orientation=Gtk.Orientation.VERTICAL,spacing=6)

        self.scroll = Gtk.ScrolledWindow()
        self.scroll.set_hexpand(True)
        self.scroll.set_vexpand(True)
        self.output = Gtk.TextView()
        self.op_buf = self.output.get_buffer()
        self.op_buf.set_text(result)
        self.output.set_editable(False)
        self.scroll.set_size_request(500, 500)
        self.scroll.add(self.output)
        self.vbox_op.pack_start(self.scroll, True, False, 0)

        box = self.get_content_area()
        box.add(self.vbox_op)
        self.show_all()

class tic_inp(Gtk.Dialog):

    def __init__(self, parent, title):
        Gtk.Dialog.__init__(self, title, parent, 0,
            (Gtk.STOCK_OK, Gtk.ResponseType.OK))

        self.set_default_size(350, 70)

        self.hbox = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=36)

        label = Gtk.Label("Tic interval:")
        self.hbox.pack_start(label, False, False, 0)
        adjustment = Gtk.Adjustment(0, 0, 100, 1, 10, 0)
        self.spinbutton = Gtk.SpinButton()
        self.spinbutton.set_adjustment(adjustment)
        self.hbox.pack_start(self.spinbutton, False, False, 0)

        box = self.get_content_area()
        box.add(self.hbox)
        self.show_all()

class nd_inp(Gtk.Dialog):

    def __init__(self, parent, title):
        Gtk.Dialog.__init__(self, title, parent, 0,
            (Gtk.STOCK_OK, Gtk.ResponseType.OK))

        self.set_default_size(350, 70)

        self.hbox = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=36)

        label1 = Gtk.Label("Src:")
        self.hbox.pack_start(label1, False, False, 0)
        adjustment1 = Gtk.Adjustment(0, 0, 100, 1, 10, 0)
        self.spinbutton1 = Gtk.SpinButton()
        self.spinbutton1.set_adjustment(adjustment1)
        self.spinbutton1.set_numeric(True)
        self.hbox.pack_start(self.spinbutton1, False, False, 0)

        label2 = Gtk.Label("Dest:")
        self.hbox.pack_start(label2, False, False, 0)
        adjustment2 = Gtk.Adjustment(0, 0, 100, 1, 10, 0)
        self.spinbutton2 = Gtk.SpinButton()
        self.spinbutton2.set_adjustment(adjustment2)
        self.spinbutton2.set_numeric(True)
        self.hbox.pack_start(self.spinbutton2, False, False, 0)

        label3 = Gtk.Label("Flow id:")
        self.hbox.pack_start(label3, False, False, 0)
        adjustment3 = Gtk.Adjustment(0, 0, 100, 1, 10, 0)
        self.spinbutton3 = Gtk.SpinButton()
        self.spinbutton3.set_adjustment(adjustment3)
        self.spinbutton3.set_numeric(True)
        self.hbox.pack_start(self.spinbutton3, False, False, 0)

        box = self.get_content_area()
        box.add(self.hbox)
        self.show_all()

class rd_inp(Gtk.Dialog):

    def __init__(self, parent, title):
        Gtk.Dialog.__init__(self, title, parent, 0,
            (Gtk.STOCK_OK, Gtk.ResponseType.OK))

        self.set_default_size(350, 70)

        self.hbox = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=36)

        label1 = Gtk.Label("Initial Energy:")
        self.hbox.pack_start(label1, False, False, 0)
        adjustment1 = Gtk.Adjustment(0, 0, 100, 1, 10, 0)
        self.spinbutton1 = Gtk.SpinButton()
        self.spinbutton1.set_adjustment(adjustment1)
        self.hbox.pack_start(self.spinbutton1, False, False, 0)

        box = self.get_content_area()
        box.add(self.hbox)
        self.show_all()

class rd_nd_inp(Gtk.Dialog):

    def __init__(self, parent, title):
        Gtk.Dialog.__init__(self, title, parent, 0,
            (Gtk.STOCK_OK, Gtk.ResponseType.OK))

        self.set_default_size(350, 70)

        self.hbox = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=36)

        label1 = Gtk.Label("Initial Energy:")
        self.hbox.pack_start(label1, False, False, 0)
        adjustment1 = Gtk.Adjustment(0, 0, 100, 1, 10, 0)
        self.spinbutton1 = Gtk.SpinButton()
        self.spinbutton1.set_adjustment(adjustment1)
        self.hbox.pack_start(self.spinbutton1, False, False, 0)

        label2 = Gtk.Label("Node:")
        self.hbox.pack_start(label2, False, False, 0)
        adjustment2 = Gtk.Adjustment(0, 0, 100, 1, 10, 0)
        self.spinbutton2 = Gtk.SpinButton()
        self.spinbutton2.set_adjustment(adjustment2)
        self.spinbutton2.set_numeric(True)
        self.hbox.pack_start(self.spinbutton2, False, False, 0)


        box = self.get_content_area()
        box.add(self.hbox)
        self.show_all()

class tic_nd_inp(Gtk.Dialog):

    def __init__(self, parent, title):
        Gtk.Dialog.__init__(self, title, parent, 0,
            (Gtk.STOCK_OK, Gtk.ResponseType.OK))

        self.set_default_size(450, 70)

        self.hbox = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL, spacing=36)

        label1 = Gtk.Label("Src:")
        self.hbox.pack_start(label1, False, False, 0)
        adjustment1 = Gtk.Adjustment(0, 0, 100, 1, 10, 0)
        self.spinbutton1 = Gtk.SpinButton()
        self.spinbutton1.set_adjustment(adjustment1)
        self.spinbutton1.set_numeric(True)
        self.hbox.pack_start(self.spinbutton1, False, False, 0)

        label2 = Gtk.Label("Dest:")
        self.hbox.pack_start(label2, False, False, 0)
        adjustment2 = Gtk.Adjustment(0, 0, 100, 1, 10, 0)
        self.spinbutton2 = Gtk.SpinButton()
        self.spinbutton2.set_adjustment(adjustment2)
        self.spinbutton2.set_numeric(True)
        self.hbox.pack_start(self.spinbutton2, False, False, 0)

        label3 = Gtk.Label("Flow id:")
        self.hbox.pack_start(label3, False, False, 0)
        adjustment3 = Gtk.Adjustment(0, 0, 100, 1, 10, 0)
        self.spinbutton3 = Gtk.SpinButton()
        self.spinbutton3.set_adjustment(adjustment3)
        self.spinbutton3.set_numeric(True)
        self.hbox.pack_start(self.spinbutton3, False, False, 0)

        label4 = Gtk.Label("Tic interval:")
        self.hbox.pack_start(label4, False, False, 0)
        adjustment4 = Gtk.Adjustment(0, 0, 100, 1, 10, 0)
        self.spinbutton4 = Gtk.SpinButton()
        self.spinbutton4.set_adjustment(adjustment4)
        self.hbox.pack_start(self.spinbutton4, False, False, 0)

        box = self.get_content_area()
        box.add(self.hbox)
        self.show_all()

class Main_window(Gtk.Window):

    def __init__(self):
        Gtk.Window.__init__(self, title="Automated Post Processing (APP) Tool")
        self.set_size_request(1000, 660)
        self.set_border_width(10)

        action_group = Gtk.ActionGroup("my_actions")

        self.add_file_menu_actions(action_group)
        self.add_help_menu_actions(action_group)

        uimanager = self.create_ui_manager()
        uimanager.insert_action_group(action_group)

        menubar = uimanager.get_widget("/MenuBar")

        self.vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL,spacing=6)
        self.vbox.pack_start(menubar, False, False, 0)

        self.tracefile = ""
        self.result = ""

        self.add(self.vbox)

        self.head_vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL,spacing=6)
        self.head_vbox1 = Gtk.Box(orientation=Gtk.Orientation.VERTICAL,spacing=6)
        self.head_vbox2 = Gtk.Box(orientation=Gtk.Orientation.VERTICAL,spacing=6)
        self.head_im = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.logo = Gtk.Image.new_from_file ("img/nitk_logo.png")
        self.head_im.pack_start(self.logo,True,False,0)
        self.logo_w = Gtk.Image.new_from_file ("img/wing.png")
        self.head_im.pack_start(self.logo_w,True,False,0)
        self.head_vbox1.pack_start(self.head_im,False,False,0)
        self.head_label = Gtk.Label()
        self.head_label.set_markup("<b>Designed By:\n\nWireless Information Networking Group (WING)\nNITK Surathkal</b>")
        self.head_label.set_justify(Gtk.Justification.CENTER)
        self.head_vbox2.pack_start(self.head_label,False,False,0)
        self.head_vbox.pack_start(self.head_vbox1,True,False,0)
        self.head_vbox.pack_start(self.head_vbox2,True,False,0)
        self.vbox.pack_start(self.head_vbox, False, False, 0)


        self.hbox1 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hbox2 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hbox3 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hbox4 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hbox5 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hbox6 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hbox = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hbox7 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hbox8 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)

        self.labelt = Gtk.Label()
        self.labelt.set_markup("<b>Trace File:- </b>")
        self.labelt.set_justify(Gtk.Justification.LEFT)
        self.hbox7.pack_start(self.labelt, False, False, 0)
        self.vbox.pack_start(self.hbox7, False, False, 0)

        self.labelf = Gtk.Label()
        if self.tracefile == "":
        	self.labelf.set_text("No file chosen")
        else:
        	self.labelf.set_text(self.tracefile)
        self.labelf.set_justify(Gtk.Justification.LEFT)
        self.hbox8.pack_start(self.labelf, False, False, 0)
        self.filechooser = Gtk.Button("Choose File")
        self.filechooser.connect("clicked", self.on_file_clicked)
        self.hbox8.pack_start(self.filechooser, False, False, 0)
        self.vbox.pack_start(self.hbox8, False, False, 0)

        self.label_op = Gtk.Label()
        self.label_op.set_markup("<b>Options:- </b>")
        self.label_op.set_justify(Gtk.Justification.CENTER)
        self.hbox_op = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hbox_op.pack_start(self.label_op, True, False, 0)
        self.vbox.pack_start(self.hbox_op, False, False, 0)

        self.opt_vbox = Gtk.Box(orientation=Gtk.Orientation.VERTICAL,spacing=6)
        self.hbox_grid13 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=20)
        self.hbox_grid24 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=20)
        self.hbox_grid5 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=20)
        self.opt_vbox.pack_start(self.hbox_grid13, False, False, 0)
        self.opt_vbox.pack_start(self.hbox_grid24, False, False, 0)
        self.opt_vbox.pack_start(self.hbox_grid5, False, False, 0)

        self.grid1 = Gtk.Grid()
        self.thr_lbl = Gtk.Label()
        self.thr_lbl.set_markup("<b>Throughput</b>")
        self.thr_lbl.set_justify(Gtk.Justification.CENTER)
        self.hb11 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb11.pack_start(self.thr_lbl,True,False,0)
        self.grid1.attach(self.hb11,1,0,2,1)
        self.avg_thr_lbl = Gtk.Label()
        self.avg_thr_lbl.set_text("Average Throughput       ")
        self.avg_thr_lbl.set_justify(Gtk.Justification.LEFT)
        self.hb12 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb12.pack_start(self.avg_thr_lbl,False,False,0)
        self.grid1.attach_next_to(self.hb12,self.hb11,Gtk.PositionType.BOTTOM,1,1)
        self.avg_thr_flw_lbl = Gtk.Label()
        self.avg_thr_flw_lbl.set_text("Average Throughput for specific flow     ")
        self.avg_thr_flw_lbl.set_justify(Gtk.Justification.LEFT)
        self.hb13 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb13.pack_start(self.avg_thr_flw_lbl,False,False,0)
        self.grid1.attach_next_to(self.hb13,self.hb12,Gtk.PositionType.BOTTOM,1,1)
        self.ins_thr_lbl = Gtk.Label()
        self.ins_thr_lbl.set_text("Instantaneous Throughput     ")
        self.ins_thr_lbl.set_justify(Gtk.Justification.LEFT)
        self.hb14 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb14.pack_start(self.ins_thr_lbl,False,False,0)
        self.grid1.attach_next_to(self.hb14,self.hb13,Gtk.PositionType.BOTTOM,1,1)
        
        self.ins_thr_flw_lbl = Gtk.Label()
        self.ins_thr_flw_lbl.set_text("Instantaneous Throughput for specific flow   ")
        self.avg_thr_flw_lbl.set_justify(Gtk.Justification.LEFT)
        self.hb15 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb15.pack_start(self.ins_thr_flw_lbl,False,False,0)
        self.grid1.attach_next_to(self.hb15,self.hb14,Gtk.PositionType.BOTTOM,1,1)
        
        self.av_th_sw = Gtk.Switch()
        self.grid1.attach_next_to(self.av_th_sw,self.hb12,Gtk.PositionType.RIGHT,1,1)
        self.av_th_flw_sw = Gtk.Switch()
        self.grid1.attach_next_to(self.av_th_flw_sw,self.hb13,Gtk.PositionType.RIGHT,1,1)
        self.in_th_sw = Gtk.Switch()
        self.grid1.attach_next_to(self.in_th_sw,self.hb14,Gtk.PositionType.RIGHT,1,1)
        self.in_th_flw_sw = Gtk.Switch()
        self.grid1.attach_next_to(self.in_th_flw_sw,self.hb15,Gtk.PositionType.RIGHT,1,1)
        self.in_th_tg = Gtk.CheckButton("Generate Graph")
        self.in_th_flw_tg = Gtk.CheckButton("Generate Graph")
        self.grid1.attach_next_to(self.in_th_tg,self.in_th_sw,Gtk.PositionType.RIGHT,1,1)
        self.grid1.attach_next_to(self.in_th_flw_tg,self.in_th_flw_sw,Gtk.PositionType.RIGHT,1,1)
        self.hbox_grid13.pack_start(self.grid1,True,True,0)


        self.grid2 = Gtk.Grid()
        self.gd_lbl = Gtk.Label()
        self.gd_lbl.set_markup("<b>Goodput</b>")
        self.gd_lbl.set_justify(Gtk.Justification.CENTER)
        self.hb21 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb21.pack_start(self.gd_lbl,True,False,0)
        self.grid2.attach(self.hb21,1,0,2,1)
        self.avg_gd_lbl = Gtk.Label()
        self.avg_gd_lbl.set_text("Average Goodput       ")
        self.avg_gd_lbl.set_justify(Gtk.Justification.LEFT)
        self.hb22 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb22.pack_start(self.avg_gd_lbl,False,False,0)
        self.grid2.attach_next_to(self.hb22,self.hb21,Gtk.PositionType.BOTTOM,1,1)
        self.avg_gd_flw_lbl = Gtk.Label()
        self.avg_gd_flw_lbl.set_text("Average Goodput for specific flow     ")
        self.avg_gd_flw_lbl.set_justify(Gtk.Justification.LEFT)
        self.hb23 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb23.pack_start(self.avg_gd_flw_lbl,False,False,0)
        self.grid2.attach_next_to(self.hb23,self.hb22,Gtk.PositionType.BOTTOM,1,1)
        self.ins_gd_lbl = Gtk.Label()
        self.ins_gd_lbl.set_text("Instantaneous Goodput     ")
        self.ins_gd_lbl.set_justify(Gtk.Justification.LEFT)
        self.hb24 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb24.pack_start(self.ins_gd_lbl,False,False,0)
        self.grid2.attach_next_to(self.hb24,self.hb23,Gtk.PositionType.BOTTOM,1,1)
        
        self.ins_gd_flw_lbl = Gtk.Label()
        self.ins_gd_flw_lbl.set_text("Instantaneous Goodput for specific flow   ")
        self.avg_gd_flw_lbl.set_justify(Gtk.Justification.LEFT)
        self.hb25 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb25.pack_start(self.ins_gd_flw_lbl,False,False,0)
        self.grid2.attach_next_to(self.hb25,self.hb24,Gtk.PositionType.BOTTOM,1,1)
        
        self.av_gd_sw = Gtk.Switch()
        self.grid2.attach_next_to(self.av_gd_sw,self.hb22,Gtk.PositionType.RIGHT,1,1)
        self.av_gd_flw_sw = Gtk.Switch()
        self.grid2.attach_next_to(self.av_gd_flw_sw,self.hb23,Gtk.PositionType.RIGHT,1,1)
        self.in_gd_sw = Gtk.Switch()
        self.grid2.attach_next_to(self.in_gd_sw,self.hb24,Gtk.PositionType.RIGHT,1,1)
        self.in_gd_flw_sw = Gtk.Switch()
        self.grid2.attach_next_to(self.in_gd_flw_sw,self.hb25,Gtk.PositionType.RIGHT,1,1)
        self.in_gd_tg = Gtk.CheckButton("Generate Graph")
        self.in_gd_flw_tg = Gtk.CheckButton("Generate Graph")
        self.grid2.attach_next_to(self.in_gd_tg,self.in_gd_sw,Gtk.PositionType.RIGHT,1,1)
        self.grid2.attach_next_to(self.in_gd_flw_tg,self.in_gd_flw_sw,Gtk.PositionType.RIGHT,1,1)
        self.hbox_grid24.pack_start(self.grid2,True,True,0)

        self.grid3 = Gtk.Grid()
        self.dl_lbl = Gtk.Label()
        self.dl_lbl.set_markup("<b>Delay</b>")
        self.dl_lbl.set_justify(Gtk.Justification.CENTER)
        self.hb31 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb31.pack_start(self.dl_lbl,True,False,0)
        self.grid3.attach(self.hb31,1,0,2,1)
        self.avg_dl_lbl = Gtk.Label()
        self.avg_dl_lbl.set_text("Average Delay       ")
        self.avg_dl_lbl.set_justify(Gtk.Justification.LEFT)
        self.hb32 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb32.pack_start(self.avg_dl_lbl,False,False,0)
        self.grid3.attach_next_to(self.hb32,self.hb31,Gtk.PositionType.BOTTOM,1,1)
        self.avg_dl_flw_lbl = Gtk.Label()
        self.avg_dl_flw_lbl.set_text("Average Delay for specific flow     ")
        self.avg_dl_flw_lbl.set_justify(Gtk.Justification.LEFT)
        self.hb33 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb33.pack_start(self.avg_dl_flw_lbl,False,False,0)
        self.grid3.attach_next_to(self.hb33,self.hb32,Gtk.PositionType.BOTTOM,1,1)
        self.ins_dl_lbl = Gtk.Label()
        self.ins_dl_lbl.set_text("Instantaneous Delay     ")
        self.ins_dl_lbl.set_justify(Gtk.Justification.LEFT)
        self.hb34 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb34.pack_start(self.ins_dl_lbl,False,False,0)
        self.grid3.attach_next_to(self.hb34,self.hb33,Gtk.PositionType.BOTTOM,1,1)
        
        self.ins_dl_flw_lbl = Gtk.Label()
        self.ins_dl_flw_lbl.set_text("Instantaneous Delay for specific flow   ")
        self.avg_dl_flw_lbl.set_justify(Gtk.Justification.LEFT)
        self.hb35 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb35.pack_start(self.ins_dl_flw_lbl,False,False,0)
        self.grid3.attach_next_to(self.hb35,self.hb34,Gtk.PositionType.BOTTOM,1,1)
        
        self.av_dl_sw = Gtk.Switch()
        self.grid3.attach_next_to(self.av_dl_sw,self.hb32,Gtk.PositionType.RIGHT,1,1)
        self.av_dl_flw_sw = Gtk.Switch()
        self.grid3.attach_next_to(self.av_dl_flw_sw,self.hb33,Gtk.PositionType.RIGHT,1,1)
        self.in_dl_sw = Gtk.Switch()
        self.grid3.attach_next_to(self.in_dl_sw,self.hb34,Gtk.PositionType.RIGHT,1,1)
        self.in_dl_flw_sw = Gtk.Switch()
        self.grid3.attach_next_to(self.in_dl_flw_sw,self.hb35,Gtk.PositionType.RIGHT,1,1)
        self.in_dl_tg = Gtk.CheckButton("Generate Graph")
        self.in_dl_flw_tg = Gtk.CheckButton("Generate Graph")
        self.grid3.attach_next_to(self.in_dl_tg,self.in_dl_sw,Gtk.PositionType.RIGHT,1,1)
        self.grid3.attach_next_to(self.in_dl_flw_tg,self.in_dl_flw_sw,Gtk.PositionType.RIGHT,1,1)
        self.hbox_grid13.pack_start(self.grid3,True,False,0)

        self.grid4 = Gtk.Grid()
        self.jt_lbl = Gtk.Label()
        self.jt_lbl.set_markup("<b>Jitter</b>")
        self.jt_lbl.set_justify(Gtk.Justification.CENTER)
        self.hb41 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb41.pack_start(self.jt_lbl,True,False,0)
        self.grid4.attach(self.hb41,1,0,2,1)
        self.avg_jt_lbl = Gtk.Label()
        self.avg_jt_lbl.set_text("Average Jitter       ")
        self.avg_jt_lbl.set_justify(Gtk.Justification.LEFT)
        self.hb42 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb42.pack_start(self.avg_jt_lbl,False,False,0)
        self.grid4.attach_next_to(self.hb42,self.hb41,Gtk.PositionType.BOTTOM,1,1)
        self.avg_jt_flw_lbl = Gtk.Label()
        self.avg_jt_flw_lbl.set_text("Average Jitter for specific flow     ")
        self.avg_jt_flw_lbl.set_justify(Gtk.Justification.LEFT)
        self.hb43 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb43.pack_start(self.avg_jt_flw_lbl,False,False,0)
        self.grid4.attach_next_to(self.hb43,self.hb42,Gtk.PositionType.BOTTOM,1,1)
        self.ins_jt_lbl = Gtk.Label()
        self.ins_jt_lbl.set_text("Instantaneous Jitter     ")
        self.ins_jt_lbl.set_justify(Gtk.Justification.LEFT)
        self.hb44 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb44.pack_start(self.ins_jt_lbl,False,False,0)
        self.grid4.attach_next_to(self.hb44,self.hb43,Gtk.PositionType.BOTTOM,1,1)
        
        self.ins_jt_flw_lbl = Gtk.Label()
        self.ins_jt_flw_lbl.set_text("Instantaneous Jitter for specific flow   ")
        self.avg_jt_flw_lbl.set_justify(Gtk.Justification.LEFT)
        self.hb45 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hb45.pack_start(self.ins_jt_flw_lbl,False,False,0)
        self.grid4.attach_next_to(self.hb45,self.hb44,Gtk.PositionType.BOTTOM,1,1)
        
        self.av_jt_sw = Gtk.Switch()
        self.grid4.attach_next_to(self.av_jt_sw,self.hb42,Gtk.PositionType.RIGHT,1,1)
        self.av_jt_flw_sw = Gtk.Switch()
        self.grid4.attach_next_to(self.av_jt_flw_sw,self.hb43,Gtk.PositionType.RIGHT,1,1)
        self.in_jt_sw = Gtk.Switch()
        self.grid4.attach_next_to(self.in_jt_sw,self.hb44,Gtk.PositionType.RIGHT,1,1)
        self.in_jt_flw_sw = Gtk.Switch()
        self.grid4.attach_next_to(self.in_jt_flw_sw,self.hb45,Gtk.PositionType.RIGHT,1,1)
        self.in_jt_tg = Gtk.CheckButton("Generate Graph")
        self.in_jt_flw_tg = Gtk.CheckButton("Generate Graph")
        self.grid4.attach_next_to(self.in_jt_tg,self.in_jt_sw,Gtk.PositionType.RIGHT,1,1)
        self.grid4.attach_next_to(self.in_jt_flw_tg,self.in_jt_flw_sw,Gtk.PositionType.RIGHT,1,1)
        self.hbox_grid24.pack_start(self.grid4,True,False,0)

        self.vbox.pack_start(self.opt_vbox, False, True, 0)

        self.label_ms = Gtk.Label()
        self.label_ms.set_markup("<b>Miscellaneous</b>")
        self.label_ms.set_justify(Gtk.Justification.CENTER)
        self.hbox_ms = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hbox_ms.pack_start(self.label_ms, True, False, 0)
        self.vbox.pack_start(self.hbox_ms, False, False, 0)

        self.hbox_ms_gr = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.vbox.pack_start(self.hbox_ms_gr, False, False, 0)        

        self.gridm1 = Gtk.Grid()
        self.res_en_lbl = Gtk.Label()
        self.res_en_lbl.set_text("Average Residual Energy of network      ")
        self.res_en_lbl.set_justify(Gtk.Justification.LEFT)
        self.hbm1 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hbm1.pack_start(self.res_en_lbl,False,False,0)
        self.gridm1.attach(self.hbm1,1,0,1,1)
        self.res_en_nd_lbl = Gtk.Label()
        self.res_en_nd_lbl.set_text("Residual Energy of a node      ")
        self.res_en_nd_lbl.set_justify(Gtk.Justification.LEFT)
        self.hbm2 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hbm2.pack_start(self.res_en_nd_lbl,False,False,0)
        self.gridm1.attach_next_to(self.hbm2,self.hbm1,Gtk.PositionType.BOTTOM,1,1)
        # self.res_en_in_lbl = Gtk.Label()
        # self.res_en_in_lbl.set_text("Instantaneous Residual Energy of a node      ")
        # self.res_en_in_lbl.set_justify(Gtk.Justification.LEFT)
        # self.hbm3 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        # self.hbm3.pack_start(self.res_en_in_lbl,False,False,0)
        # self.gridm1.attach_next_to(self.hbm3,self.hbm2,Gtk.PositionType.BOTTOM,1,1)
        self.av_rd_en = Gtk.Switch()
        self.gridm1.attach_next_to(self.av_rd_en,self.hbm1,Gtk.PositionType.RIGHT,1,1)
        self.av_rd_nd = Gtk.Switch()
        self.gridm1.attach_next_to(self.av_rd_nd,self.hbm2,Gtk.PositionType.RIGHT,1,1)
        # self.in_rd_en = Gtk.Switch()
        # self.gridm1.attach_next_to(self.in_rd_en,self.hbm3,Gtk.PositionType.RIGHT,1,1)
        # self.in_rd_en_tg = Gtk.CheckButton("Generate Graph")
        # self.gridm1.attach_next_to(self.in_rd_en_tg,self.in_rd_en,Gtk.PositionType.RIGHT,1,1)
        self.hbox_ms_gr.pack_start(self.gridm1,False,False,0)

        self.gridm2 = Gtk.Grid()
        self.pkt_dl_lbl = Gtk.Label()
        self.pkt_dl_lbl.set_text("Packet Delivery Ratio      ")
        self.pkt_dl_lbl.set_justify(Gtk.Justification.LEFT)
        self.hbm4 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hbm4.pack_start(self.pkt_dl_lbl,False,False,0)
        self.gridm2.attach(self.hbm4,1,0,1,1)
        self.nrm_rt_lbl = Gtk.Label()
        self.nrm_rt_lbl.set_text("Normalized Routing Load      ")
        self.nrm_rt_lbl.set_justify(Gtk.Justification.LEFT)
        self.hbm5 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        self.hbm5.pack_start(self.nrm_rt_lbl,False,False,0)
        self.gridm2.attach_next_to(self.hbm5,self.hbm4,Gtk.PositionType.BOTTOM,1,1)
        # self.nm_rt_pkt_lbl = Gtk.Label()
        # self.nm_rt_pkt_lbl.set_text("Number of Retransmitted Packets      ")
        # self.nm_rt_pkt_lbl.set_justify(Gtk.Justification.LEFT)
        # self.hbm6 = Gtk.Box(orientation=Gtk.Orientation.HORIZONTAL,spacing=6)
        # self.hbm6.pack_start(self.nm_rt_pkt_lbl,False,False,0)
        # self.gridm2.attach_next_to(self.hbm6,self.hbm5,Gtk.PositionType.BOTTOM,1,1)
        self.pkt_dl_sw = Gtk.Switch()
        self.gridm2.attach_next_to(self.pkt_dl_sw,self.hbm4,Gtk.PositionType.RIGHT,1,1)
        self.nrm_rt_sw = Gtk.Switch()
        self.gridm2.attach_next_to(self.nrm_rt_sw,self.hbm5,Gtk.PositionType.RIGHT,1,1)
        # self.nm_rt_sw = Gtk.Switch()
        # self.gridm2.attach_next_to(self.nm_rt_sw,self.hbm6,Gtk.PositionType.RIGHT,1,1)
        self.hbox_ms_gr.pack_start(self.gridm2,True,False,0)
        
        self.vbox.pack_start(self.hbox6, False,True, 0)

        self.button = Gtk.Button(label="Analyze")
        self.button.connect("clicked", self.on_analyze_clicked)
        self.hbox6.pack_start(self.button, True, False, 0)

    def add_file_menu_actions(self, action_group):
        action_group.add_actions([
            ("FileMenu", None, "File"),
            ("Tracefile", Gtk.STOCK_OPEN, "Tracefile", None, None,
             self.on_file_clicked),
            ("FileQuit", Gtk.STOCK_QUIT, "Quit", None, None,
             self.on_menu_file_quit)
        ])

    def add_help_menu_actions(self, action_group):
        action_group.add_actions([
            ("HelpMenu", None, "Help"),
            ("Documentation", None, "Documentation", None, None,
             self.on_menu_others),
            ("Credits", None, "Credits", None, None,
             self.on_menu_credits),
            ("Help", Gtk.STOCK_HELP, "Help", None, None,
             self.on_menu_others)
        ])


    def create_ui_manager(self):
        uimanager = Gtk.UIManager()

        # Throws exception if something went wrong
        uimanager.add_ui_from_string(UI_INFO)

        # Add the accelerator group to the toplevel window
        accelgroup = uimanager.get_accel_group()
        self.add_accel_group(accelgroup)
        return uimanager


    def on_menu_file_quit(self, widget):
        Gtk.main_quit()

    def on_menu_others(self, widget):
        print("Menu item " + widget.get_name() + " was selected")

    def on_menu_credits(self, widget):
        dialog = Gtk.MessageDialog(self, 0, Gtk.MessageType.INFO,
            Gtk.ButtonsType.OK, "Automated Post Processing (APP) Tool\n\nDesigned By:\nWireless Information Networking Group (WING)\nNITK Surathkal")
        dialog.format_secondary_text(
            "\nMembers:-\n1) Sayan Paul\n2) Jay Priyadarshi\n3)Sasank Channapragada\n4) Spriha Awinpushpam\n5) Mohit P. Tahiliani")
        dialog.run()

        dialog.destroy()

    def on_analyze_clicked(self, widget):
        if len(self.tracefile) == 0:

            dialog = Gtk.MessageDialog(self, 0, Gtk.MessageType.ERROR, Gtk.ButtonsType.OK, "TraceFile not chosen")
            dialog.run()
            dialog.destroy()

        else:

            self.result = ""

            #Check individual options

            if self.av_th_sw.get_active():
                self.result += "--Average throughput--\n"
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/Avg_Tput.awk",self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                self.result += "\n".join(p.stdout.readlines()) + "\n"
                self.result += "\n---------------------\n"


            if  self.av_th_flw_sw.get_active():
                self.result += "--Average throughput for particular flow--\n"
                dlg = nd_inp(self,"Average Throughput for particular flow")
                rsp = dlg.run()
                if rsp == Gtk.ResponseType.OK:
                    src = dlg.spinbutton1.get_value_as_int()
                    dest = dlg.spinbutton2.get_value_as_int()
                    flow = dlg.spinbutton3.get_value_as_int()
                else:
                    src=dest=flow=0
                dlg.destroy()
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/Avg_Tput_Flow.awk","src="+str(src),"dest="+str(dest),"flow="+str(flow),self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                self.result += "\n".join(p.stdout.readlines()) + "\n"
                self.result += "\n---------------------\n"

            if  self.in_th_sw.get_active():
                self.result += "--Instantaneous throughput--\n"
                dlg = tic_inp(self,"Instantaneous Throughput")
                rsp = dlg.run()
                if rsp == Gtk.ResponseType.OK:
                    tic = dlg.spinbutton.get_value()
                else:
                    tic=0.1
                dlg.destroy()
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/Instnt_Tput.awk","tic="+str(tic),self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                xy = p.stdout.readlines()
                self.result += "\n".join(xy)
                if self.in_th_tg.get_active():
                    xl=[]
                    yl=[]
                    for l in xy:
                        try:
                            x,y = map(float,l.strip().split())
                            xl.append(x)
                            yl.append(y)
                        except:
                            h=1
                    plt.figure(1)
                    plt.plot(xl,yl)
                    plt.plot(xl,yl,'ro')
                    plt.xlabel("Time interval")
                    plt.ylabel("Throughput")
                    plt.title("Instantaneous Throughput")
                    plt.axis([0,(max(xl,key=float)),0,max(yl,key=float)+100])
                    plt.show()
                self.result += "\n---------------------\n"

            if  self.in_th_flw_sw.get_active():
                self.result += "--Instantaneous throughput for particular flow--\n"
                dlg = tic_nd_inp(self,"Instantaneous Throughput for particular flow")
                rsp = dlg.run()
                if rsp == Gtk.ResponseType.OK:
                    src = dlg.spinbutton1.get_value_as_int()
                    dest = dlg.spinbutton2.get_value_as_int()
                    flow = dlg.spinbutton3.get_value_as_int()
                    tic = dlg.spinbutton4.get_value()
                else:
                    src=dest=flow=0
                    tic = 0.1
                dlg.destroy()
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/Instnt_Tput_Flow.awk","src="+str(src),"dest="+str(dest),"flow="+str(flow),"tic="+str(tic),self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                xy = p.stdout.readlines()
                self.result += "\n".join(xy)
                if self.in_th_flw_tg.get_active():
                    xl=[]
                    yl=[]
                    for l in xy:
                        try:
                            x,y = map(float,l.strip().split())
                            xl.append(x)
                            yl.append(y)
                        except:
                            h=1
                    plt.figure(4)
                    plt.plot(xl,yl)
                    plt.plot(xl,yl,'ro')
                    plt.xlabel("Time interval")
                    plt.ylabel("Throughput")
                    plt.title("Instantaneous Throughput for flow=%d, src=%d, dest=%d"%(flow,src,dest))
                    plt.axis([0,(max(xl,key=float)),0,max(yl,key=float)+100])
                    plt.show()
                self.result += "\n---------------------\n"

            if  self.av_gd_sw.get_active():
                self.result += "--Average goodput--\n"
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/Avg_Gdput.awk","pkt=1000",self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                self.result += "\n".join(p.stdout.readlines()) + "\n"
                self.result += "\n---------------------\n"


            if  self.av_gd_flw_sw.get_active():
                self.result += "--Average goodput for particular flow--\n"
                dlg = nd_inp(self,"Average Goodput for particular flow")
                rsp = dlg.run()
                if rsp == Gtk.ResponseType.OK:
                    src = dlg.spinbutton1.get_value_as_int()
                    dest = dlg.spinbutton2.get_value_as_int()
                    flow = dlg.spinbutton3.get_value_as_int()
                else:
                    src=dest=flow=0
                dlg.destroy()
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/Avg_Gdput_Flow.awk","pkt=1000","src="+str(src),"dest="+str(dest),"flow="+str(flow),self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                self.result += "\n".join(p.stdout.readlines()) + "\n"
                self.result += "\n---------------------\n"

            if  self.in_gd_sw.get_active():
                self.result += "--Instantaneous goodput--\n"
                dlg = tic_inp(self,"Instantaneous Goodput")
                rsp = dlg.run()
                if rsp == Gtk.ResponseType.OK:
                    tic = dlg.spinbutton.get_value()
                else:
                    tic=0.1
                dlg.destroy()
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/Instnt_Gdput.awk","pkt=1000","tic="+str(tic),self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                xy = p.stdout.readlines()
                self.result += "\n".join(xy)
                if self.in_gd_tg.get_active():
                    xl=[]
                    yl=[]
                    for l in xy:
                        try:
                            x,y = map(float,l.strip().split())
                            xl.append(x)
                            yl.append(y)
                        except:
                            h=1
                    plt.figure(3)
                    plt.plot(xl,yl)
                    plt.plot(xl,yl,'ro')
                    plt.xlabel("Time interval")
                    plt.ylabel("Goodput")
                    plt.title("Instantaneous Goodput")
                    plt.axis([0,(max(xl,key=float)),0,max(yl,key=float)+100])
                    plt.show()
                self.result += "\n---------------------\n"

            if  self.in_gd_flw_sw.get_active():
                self.result += "--Instantaneous goodput for particular flow--\n"
                dlg = tic_nd_inp(self,"Instantaneous Goodput for particular flow")
                rsp = dlg.run()
                if rsp == Gtk.ResponseType.OK:
                    src = dlg.spinbutton1.get_value_as_int()
                    dest = dlg.spinbutton2.get_value_as_int()
                    flow = dlg.spinbutton3.get_value_as_int()
                    tic = dlg.spinbutton4.get_value()
                else:
                    src=dest=flow=0
                    tic = 0.1
                dlg.destroy()
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/Instnt_Gdput_Flow.awk","pkt=1000","src="+str(src),"dest="+str(dest),"flow="+str(flow),"tic="+str(tic),self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                xy = p.stdout.readlines()
                self.result += "\n".join(xy)
                if self.in_gd_flw_tg.get_active():
                    xl=[]
                    yl=[]
                    for l in xy:
                        try:
                            x,y = map(float,l.strip().split())
                            xl.append(x)
                            yl.append(y)
                        except:
                            h=1
                    plt.figure(4)
                    plt.plot(xl,yl)
                    plt.plot(xl,yl,'ro')
                    plt.xlabel("Time interval")
                    plt.ylabel("Goodput")
                    plt.title("Instantaneous Goodput for flow=%d, src=%d, dest=%d"%(flow,src,dest))
                    plt.axis([0,(max(xl,key=float)),0,max(yl,key=float)+100])
                    plt.show()
                self.result += "\n---------------------\n"


            if  self.av_dl_sw.get_active():
                self.result += "--Average Delay--\n"
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/Avg_Del.awk","pkt=1000",self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                self.result += "\n".join(p.stdout.readlines()) + "\n"
                self.result += "\n---------------------\n"


            if  self.av_dl_flw_sw.get_active():
                self.result += "--Average delay for particular flow--\n"
                dlg = nd_inp(self,"Average Delay for particular flow")
                rsp = dlg.run()
                if rsp == Gtk.ResponseType.OK:
                    src = dlg.spinbutton1.get_value_as_int()
                    dest = dlg.spinbutton2.get_value_as_int()
                    flow = dlg.spinbutton3.get_value_as_int()
                else:
                    src=dest=flow=0
                dlg.destroy()
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/Avg_Del_Flow.awk","pkt=1000","src="+str(src),"dest="+str(dest),"flow="+str(flow),self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                self.result += "\n".join(p.stdout.readlines()) + "\n"
                self.result += "\n---------------------\n"

            if  self.in_dl_sw.get_active():
                self.result += "--Instantaneous delay--\n"
                dlg = tic_inp(self,"Instantaneous Delay")
                rsp = dlg.run()
                if rsp == Gtk.ResponseType.OK:
                    tic = dlg.spinbutton.get_value()
                else:
                    tic=0.1
                dlg.destroy()
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/Instnt_Del.awk","pkt=1000","tic="+str(tic),self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                xy = p.stdout.readlines()
                self.result += "\n".join(xy)
                if self.in_dl_tg.get_active():
                    xl=[]
                    yl=[]
                    for l in xy:
                        try:
                            x,y = map(float,l.strip().split())
                            xl.append(x)
                            yl.append(y)
                        except:
                            h=1
                    plt.figure(3)
                    plt.plot(xl,yl)
                    plt.plot(xl,yl,'ro')
                    plt.xlabel("Time interval")
                    plt.ylabel("Delay")
                    plt.title("Instantaneous Delay")
                    plt.axis([0,(max(xl,key=float)),0,max(yl,key=float)+1])
                    plt.show()
                self.result += "\n---------------------\n"

            if  self.in_dl_flw_sw.get_active():
                self.result += "--Instantaneous delay for particular flow--\n"
                dlg = tic_nd_inp(self,"Instantaneous Delay for particular flow")
                rsp = dlg.run()
                if rsp == Gtk.ResponseType.OK:
                    src = dlg.spinbutton1.get_value_as_int()
                    dest = dlg.spinbutton2.get_value_as_int()
                    flow = dlg.spinbutton3.get_value_as_int()
                    tic = dlg.spinbutton4.get_value()
                else:
                    src=dest=flow=0
                    tic = 0.1
                dlg.destroy()
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/Instnt_Del_Flow.awk","pkt=1000","src="+str(src),"dest="+str(dest),"flow="+str(flow),"tic="+str(tic),self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                xy = p.stdout.readlines()
                self.result += "\n".join(xy)
                if self.in_dl_flw_tg.get_active():
                    xl=[]
                    yl=[]
                    for l in xy:
                        try:
                            x,y = map(float,l.strip().split())
                            xl.append(x)
                            yl.append(y)
                        except:
                            h=1
                    plt.figure(4)
                    plt.plot(xl,yl)
                    plt.plot(xl,yl,'ro')
                    plt.xlabel("Time interval")
                    plt.ylabel("Delay")
                    plt.title("Instantaneous Delay for flow=%d, src=%d, dest=%d"%(flow,src,dest))
                    plt.axis([0,(max(xl,key=float)),0,max(yl,key=float)+1])
                    plt.show()
                self.result += "\n---------------------\n"


            if  self.av_jt_sw.get_active():
                self.result += "--Average jitter--\n"
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/Avg_Jit.awk","pkt=1000",self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                self.result += "\n".join(p.stdout.readlines()) + "\n"
                self.result += "\n---------------------\n"


            if  self.av_jt_flw_sw.get_active():
                self.result += "--Average jitter for particular flow--\n"
                dlg = nd_inp(self,"Average Jitter for particular flow")
                rsp = dlg.run()
                if rsp == Gtk.ResponseType.OK:
                    src = dlg.spinbutton1.get_value_as_int()
                    dest = dlg.spinbutton2.get_value_as_int()
                    flow = dlg.spinbutton3.get_value_as_int()
                else:
                    src=dest=flow=0
                dlg.destroy()
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/Avg_Jit_Flow.awk","pkt=1000","src="+str(src),"dest="+str(dest),"flow="+str(flow),self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                self.result += "\n".join(p.stdout.readlines()) + "\n"
                self.result += "\n---------------------\n"

            if  self.in_jt_sw.get_active():
                self.result += "--Instantaneous jitter--\n"
                dlg = tic_inp(self,"Instantaneous Jitter")
                rsp = dlg.run()
                if rsp == Gtk.ResponseType.OK:
                    tic = dlg.spinbutton.get_value()
                else:
                    tic=0.1
                dlg.destroy()
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/Instnt_Jit.awk","pkt=1000","tic="+str(tic),self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                xy = p.stdout.readlines()
                self.result += "\n".join(xy)
                if self.in_jt_tg.get_active():
                    xl=[]
                    yl=[]
                    for l in xy:
                        try:
                            x,y = map(float,l.strip().split())
                            xl.append(x)
                            yl.append(y)
                        except:
                            h=1
                    plt.figure(3)
                    plt.plot(xl,yl)
                    plt.plot(xl,yl,'ro')
                    plt.xlabel("Time interval")
                    plt.ylabel("Jitter")
                    plt.title("Instantaneous Jitter")
                    plt.axis([0,(max(xl,key=float)),0,max(yl,key=float)+1])
                    plt.show()
                self.result += "\n---------------------\n"

            if  self.in_jt_flw_sw.get_active():
                self.result += "--Instantaneous jitter for particular flow--\n"
                dlg = tic_nd_inp(self,"Instantaneous Jitter for particular flow")
                rsp = dlg.run()
                if rsp == Gtk.ResponseType.OK:
                    src = dlg.spinbutton1.get_value_as_int()
                    dest = dlg.spinbutton2.get_value_as_int()
                    flow = dlg.spinbutton3.get_value_as_int()
                    tic = dlg.spinbutton4.get_value()
                else:
                    src=dest=flow=0
                    tic = 0.1
                dlg.destroy()
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/Instnt_Jit_Flow.awk","pkt=1000","src="+str(src),"dest="+str(dest),"flow="+str(flow),"tic="+str(tic),self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                xy = p.stdout.readlines()
                self.result += "\n".join(xy)
                if self.in_jt_flw_tg.get_active():
                    xl=[]
                    yl=[]
                    for l in xy:
                        try:
                            x,y = map(float,l.strip().split())
                            xl.append(x)
                            yl.append(y)
                        except:
                            h=1
                    plt.figure(4)
                    plt.plot(xl,yl)
                    plt.plot(xl,yl,'ro')
                    plt.xlabel("Time interval")
                    plt.ylabel("Jitter")
                    plt.title("Instantaneous Jitter for flow=%d, src=%d, dest=%d"%(flow,src,dest))
                    plt.axis([0,(max(xl,key=float)),0,max(yl,key=float)+1])
                    plt.show()
                self.result += "\n---------------------\n"

            if self.pkt_dl_sw.get_active():
                self.result+="Packet Delivery Ratio\n"
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/Pkt.awk",self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                self.result += "\n".join(p.stdout.readlines())
                self.result += "\n---------------------\n"

            if self.nrm_rt_sw.get_active():
                self.result+="Normalised Routing Load\n"
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/nrm_rt_ld.awk",self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                self.result += "\n".join(p.stdout.readlines())
                self.result += "\n---------------------\n"

            if self.av_rd_en.get_active():
                self.result += "Average Residual Energy\n"
                dlg = rd_inp(self,"Average Residual Energy")
                rsp = dlg.run()
                if rsp == Gtk.ResponseType.OK:
                    initenergy = dlg.spinbutton1.get_value()
                else:
                    initenergy = 0.0

                dlg.destroy()
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/Av_res_en.awk","initenergy="+str(initenergy),self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                xy = p.stdout.readlines()
                self.result += "\n".join(xy)
                self.result += "\n---------------------\n"

            if self.av_rd_nd.get_active():
                self.result += "Residual Energy for particular node\n"
                dlg = rd_nd_inp(self,"Residual Energy for particular node")
                rsp = dlg.run()
                if rsp == Gtk.ResponseType.OK:
                    initenergy = dlg.spinbutton1.get_value()
                    node = dlg.spinbutton2.get_value()
                else:
                    initenergy = 10000
                    node = 1

                dlg.destroy()
                command = ["awk","-f",os.path.dirname(os.path.abspath(__file__))+"/Final/rd_nd_en.awk","initenergy="+str(initenergy),"node="+str(node),self.tracefile]
                p = subprocess.Popen(command,stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                xy = p.stdout.readlines()
                self.result += "\n".join(xy)
                self.result += "\n---------------------\n"


            #Show in seperate dialog

            dialog = output_dialog(self, result = self.result)
            response = dialog.run()

            if response == 66:
                save_dlg = Gtk.FileChooserDialog("Please choose a file", self,
                Gtk.FileChooserAction.SAVE,
                (Gtk.STOCK_CANCEL, Gtk.ResponseType.CANCEL,
                 Gtk.STOCK_SAVE, Gtk.ResponseType.OK))
                resp = save_dlg.run()
                if resp == Gtk.ResponseType.OK:
                    name = save_dlg.get_filename()
                    fp = open(name, "w+")
                    fp.write(self.result)
                    fp.close()
                save_dlg.destroy()
            dialog.destroy()


    def on_file_clicked(self, widget):
        dialog = Gtk.FileChooserDialog("Please choose a file", self,
            Gtk.FileChooserAction.OPEN,
            (Gtk.STOCK_CANCEL, Gtk.ResponseType.CANCEL,
             Gtk.STOCK_OPEN, Gtk.ResponseType.OK))

        self.add_filters(dialog)

        response = dialog.run()
        if response == Gtk.ResponseType.OK:
            self.tracefile = dialog.get_filename()
        elif response == Gtk.ResponseType.CANCEL:
            pass
        self.labelf.set_text(self.tracefile)
        dialog.destroy()

    def add_filters(self, dialog):

        filter_any = Gtk.FileFilter()
        filter_any.set_name("Any files")
        filter_any.add_pattern("*")
        filter_tr = Gtk.FileFilter()
        filter_tr.set_name("Trace files")
        filter_tr.add_pattern("*.tr")
        dialog.add_filter(filter_tr)
        dialog.add_filter(filter_any)
        

win = Main_window()
win.connect("delete-event", Gtk.main_quit)
win.show_all()
Gtk.main()
