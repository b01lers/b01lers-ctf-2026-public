use strict;

my $a = <STDIN>;
my $b = <STDIN>;
my $c = <STDIN>;
my $r = 1;

for my $i (1 .. $b) {
    $r = $r * $a % $c;
}

print $r;