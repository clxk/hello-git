#!/usr/bin/perl
$argc   = @ARGV;
if($argc != 1){
    print "usage:\n";
    die "perl pdf.pl inputfile\n";
}

$input  = $ARGV[0];
open (MYFILE, $input) || die ("Could not open file: $input");
foreach (<MYFILE>){
    if(/\b[A-Z]+R[n\d]?[A-Z]?\b – /){
        print $_;
    }
}
