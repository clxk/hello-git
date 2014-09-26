#!usr/bin/perl
$argc   = @ARGV;
if($argc != 1){
    print "usage:\n";
    die "perl pdf.pl inputfile\n";
}

$input  = '.txt';
system("pdftotext -layout -enc UTF-8 $ARGV[0] $input");
open (MYFILE, $input) || die ("Could not open file: $input");
#foreach (<MYFILE>){
#    if(//){
#        print $_;
#    }
#}
