#! /usr/bin/perl -w

use strict;
use Getopt::Long;

my $filename = ""; 	# File to be stripped
my $classname = "";	# Class name for method stripping

unless (@ARGV == 2) {
	print "Usage:\n";
	print " ./strip_rootcint_dict.pl --file DictionaryFile.cpp ",  
	      "--class ClasssName\n";
	print "Please specify the arguments\n"; 
	exit;
}
&GetOptions("file=s" => \$filename, "class=s" => \$classname)
	   || die "Unknown option";
if (not $filename or not $classname) {
	print "Usage:\n";
	print " ./strip_rootcint_dict.pl --file DictionaryFile.cpp ",  
	      "--class ClasssName\n";
	print "Please specify the arguments\n"; 
	exit;
}
print "Stripping class '$classname' in file '$filename'\n";

my $tag_start="void $classname"."::Streamer";
my $tag_stop="void $classname"."::ShowMembers";
my @opt = `cat $filename`;
my @array = split /\./, $filename;
my $line;
my $startstripping = 0;

open(J,">$filename") or die "Cannot overwrite ";
for $line (@opt) {
	if ($line =~/$tag_start/) {
		#print STDERR "Found $tag_start \n";
		$startstripping = 1;
	}
	elsif ($line =~/$tag_stop/) {
		#print STDERR "Found $tag_stop/ \n";
		#$line = ""; #Replace tag_stop with empty string
		$startstripping = 0;
	}
	if ($startstripping){
		#print STDERR "Line to strip: $line \n";
		#$line = ""; #Replace lines with empty string
		$line = "//$line"; #Replace lines with commented lines
	}
	print J $line;
}
close J;
