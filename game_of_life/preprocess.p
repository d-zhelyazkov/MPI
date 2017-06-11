use strict;

my $F_CHAR = ' ';

my $file = $ARGV[0];
my $width = $ARGV[1];
my $height = $ARGV[2];

open(my $input, "<", $file);
my @data = <$input>;
close($input);

my $outFile = join('_', $file, $width, $height);
open (my $output, ">", $outFile);
print $output "$width $height\n";

my $lines = @data;
my $blankLines = $height - $lines;
my $blankLine = ($F_CHAR x $width)."\n";
for (1 .. $blankLines) {
	print $output $blankLine;
}

for my $line (@data) {
    $line =~ s/\R//g;
	$line =~ s/\./$F_CHAR/g;
	$line =~ s/O/#/g;
	
	my $lineLen = length($line);
	my $blankReminder = $F_CHAR x ($width - $lineLen);
	
	#print "$lineLen " . length($blankReminder) . "\n"; 
	print $output "$line$blankReminder\n";
}


close($output);
print $outFile; 
