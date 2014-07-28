#!usr/bin/perl

$argc = @ARGV;
if($argc != 6){
    print "usage:\n";
    die "perl txt.pl inputfile bssStart bssEnd dataStart dataEnd limitSize\n";
}
$input = $ARGV[0];
$bss_s = $ARGV[1];
$bss_e = $ARGV[2];
$data_s = $ARGV[3];
$data_e = $ARGV[4];
$limit_size = $ARGV[5];
my $bss_count,$bss_total;
my $bss_count_times,$bss_total_times;
my @bss;
my $data_count,$data_total;
my $data_count_times,$data_total_times;
my @data;
open (MYFILE, $input) || die ("Could not open file: $input");
while (<MYFILE>) {
    @list = split;
    if(${list}[1] =~ /^0x/ && ${list}[0] =~ /^0x/ ||
            ${list}[2] =~ /^0x/ && ${list}[1] =~ /^0x/ && (${list}[0] =~ /^\.bss\./ || (${list}[0] =~ /^\.data/ && /^ /))){
        if(${list}[0] =~ /^\.bss\./ || (${list}[0] =~ /^\.data/ && /^ /)){
            ${list}[0] = ${list}[1];
            ${list}[1] = ${list}[2];
        }
        if(hex(${list}[0]) >= hex($bss_s) && hex(${list}[0]) < hex($bss_e)){       #bss seciton
            $bss_total += hex(${list}[1]);
            $bss_total_times++;
            if(hex(${list}[1]) >= hex($limit_size)){
                $bss_count += hex(${list}[1]);
                $bss_count_times++;
                push @bss,$_;
            }
        }

        if(hex(${list}[0]) >= hex($data_s) && hex(${list}[0]) < hex($data_e)){       #data seciton
            $data_total += hex(${list}[1]);
            $data_total_times++;
            if(hex(${list}[1]) >= hex($limit_size)){
                $data_count += hex(${list}[1]);
                $data_count_times++;
                push @data,$_;
            }
        }
    }
}

print "\nDATA SECTION\n";
print "---------------------------------------------------------------------------------------------------\n";
print @data;
print "total size:     ".data10to16($data_total)."/[$data_total_times times]\n";
print "counted size:   ".data10to16($data_count)."/[$data_count_times times](".($data_count/$data_total*100)."%/".($data_count_times/$data_total_times*100).")\n";

print "\nBSS SECTION\n";
print "---------------------------------------------------------------------------------------------------\n";
print @bss;
print "total size:     ".data10to16($bss_total)."/[$bss_total_times times]\n";
print "counted size:   ".data10to16($bss_count)."/[$bss_count_times times](".($bss_count/$bss_total*100)."%/".($bss_count_times/$bss_total_times*100).")\n";





sub data10to16{
    my($data)=@_;
    my $data16=sprintf("0x%x", $data);
    return $data16;
}

