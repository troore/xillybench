#!/usr/bin/perl
use strict;
use warnings;
use HTTP::Date;

use Tk;
require Tk::ROText;

undef $ENV{PATH};

my $oldstatus = "";

my ($mw, $textout) = setup_main_window();

die("Must run as root\n") unless ($> == 0);

open (UPTIME, "/proc/uptime") or die("Failed to open /proc/uptime\n");
my $uptimestr = <UPTIME>; close UPTIME;

our $now = time();

my ($uptime) = ($uptimestr =~ /^(\d+)/);

die("Failed to parse /proc/uptime\n") unless (defined $uptime);

our $bootuptime = $now - $uptime;

open(H, "/usr/bin/tail -f -n +1 /var/log/messages |") or die "Failed to launch tail: $!";

$mw->fileevent(\*H, 'readable', [\&fill_text_widget, $textout]);

MainLoop;

sub fill_text_widget {

  my($widget) = @_;

  my $line = <H>;
  ($line) = ($line =~ /^([^\n\r]*)/); # Untaint and chomp in one go

  return unless ($line =~ /xillybus/i);

  my ($time) = ($line =~ /^(\w+[ ]+[0-9:, ]+)[ ]+/);

  my $epochtime = str2time($time);

  # Discard messages before current bootup
  return unless ($epochtime >= $bootuptime);

  $widget->insert('end', "$line\n");
  $widget->yview('end');

}

sub setup_main_window {
  my ($main_label, $sub_label);

  my $main_window = MainWindow->new;
  $main_window->title( "Xillybus diagnostics (watching /var/log/messages)" );
  my $font = 'Courier';

  my $frame_status = $main_window->Frame( -borderwidth => 2, -relief => 'groove' );

  my $frame_log = $main_window->Frame( -borderwidth => 2, -relief => 'groove' );

  $frame_status  ->pack( -side => 'top', -padx => 2, -pady => 2, -anchor => 'n', -expand => 0, -fill => 'x'    );
  $frame_log  ->pack( -side => 'top', -padx => 2, -pady => 2, -anchor => 'n', -expand => 1, -fill => 'both' );

  $main_label = $frame_status->ROText(-font => 'Helvetica 16',
				      -height => 1,
				      -insertwidth => 0 # No cursor
				     );

  $main_label->pack( -side => 'top', -padx => 2, -pady => 0,
		     -expand => 'yes', -fill => 'both' );

  $main_label->tagConfigure('OK', -foreground => 'black');
  $main_label->tagConfigure('error', -foreground => 'red');

  $sub_label = $frame_status->ROText(-font => 'Helvetica 12',
				     -height => 3,
				     -foreground => 'black',
				     -wrap => 'word',
				     -insertwidth => 0 # No cursor
				    );

  $sub_label->pack( -side => 'bottom', -padx => 2, -pady => 0,
		    -expand => 'yes', -fill => 'both' );


  # log frame
  my $log_text = $frame_log->ROText(  -font => $font,
				      -height => 19,
				      -background => 'white',
				   );
  my $log_scrollbar = $frame_log->Scrollbar( -orient => 'vertical',
					     -command => [ 'yview' => $log_text ] );
  $log_text->configure( -yscrollcommand => [ $log_scrollbar => 'set' ] );

  $log_scrollbar->pack( -side => 'right', -padx => 2, -pady => 2, -fill => 'y' );
  $log_text ->pack( -side => 'left',  -padx => 2, -pady => 2, -fill => 'both', -expand => 1 );

  dostatus($main_label, $sub_label);

  $main_window->repeat(1000, [ \&dostatus, $main_label, $sub_label ]);

  return ($main_window, $log_text);
}

sub dostatus {

  my ($main_label, $sub_label) = @_;

  local $/;			# Slurp mode
  my $status;
  my $substatus;
  my $statuscolor;

  my @devs = </dev/xillybus_*>;

  if (@devs) {
    $status = "Xillybus has loaded properly";
    $substatus = "Looks good: The device files have been detected as /dev/xillybus_*";
    $statuscolor = 'OK';
    goto finish;
  }

  my $lspci = `/sbin/lspci -d 10ee:ebeb`;

  unless ($lspci) {
    $status = "The Xillybus PCIe endpoint was not found on the bus";
    $substatus = "There is no device on the bus with Vendor/Product IDs 10ee/ebeb, as expected. This is not a Xillybus-specific problem. The Xilinx PCIe core is either not present, misconfigured, incorrectly driven by clock, or its pins are placed incorrectly. (Was the FPGA configured when the computer powered up?)";
    $statuscolor = 'error';
    goto finish;
  }

  open (MODULES, "/proc/modules") or die("Failed to open /proc/modules\n");
  my $modulesstr = <MODULES>; close MODULES;

  unless ($modulesstr =~ /^xillybus/m) {
    $status = "The Xillybus driver (kernel module) is not loaded";
    $substatus = "Please download the Xillybus module, compile it and use insmod to load it temporarily. Or follow the instructions in the INSTALL.TXT file for an installation.";
    $statuscolor = 'error';
    goto finish;
  }

  $status = "No Xillybus device files were generated";
  $substatus = "The card was detected, but it looks like something went wrong. The log below should give a hint.";
  $statuscolor = 'error';

 finish:
  if ($oldstatus ne $status) {
    $oldstatus = $status;
    $main_label->delete('0.0', 'end');
    $main_label->insert('end', $status, $statuscolor);
    $sub_label->delete('0.0', 'end');
    $sub_label->insert('end', $substatus);
  }
}

