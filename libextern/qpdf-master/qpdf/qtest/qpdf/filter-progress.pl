use strict;
use warnings;

my $seen = 0;
while (<>)
{
    if (m/write progress: (?:10)?0\%/)
    {
        print;
    }
    elsif (m/write progress: /)
    {
        if (! $seen)
        {
            $seen = 1;
            print "....other write progress....\n";
        }
    }
    else
    {
        print;
    }
}
