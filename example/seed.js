#!/usr/bin/env seed
const Gettext = imports.gettext;
const Gtk = imports.gi.Gtk;
const LunarCalendar = imports.gi.LunarCalendar;

function TestCalendar() {
  this._init ();
}

TestCalendar.prototype = {
  _init: function () {
    this.window = new Gtk.Window ({title: "Lunar Calendar"});
    //this.window.connect("delete-event", Gtk.main_quit);
    this.window.signal.hide.connect(function () { Gtk.main_quit(); });
    this.cal = new LunarCalendar.Calendar();
    this.window.add(this.cal);
    this.window.show_all ();
  }
}

//Gettext.bindtextdomain("lunar-date", "/usr/share/locale");
//Gettext.textdomain("lunar-date");

Gtk.init (0, null);
var tc = new TestCalendar();
Gtk.main ();

