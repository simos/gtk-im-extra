#!/usr/bin/perl -w

use utf8;

my %consonants = 
(
    'ka'    =>  [ 0x0915 ],
    'kha'   =>  [ 0x0916 ],
    'ga'    =>  [ 0x0917 ],
    'gha'   =>  [ 0x0918 ],
    'nga'   =>  [ 0x0919 ],
    'ca'    =>  [ 0x091A ],
    'cha'   =>  [ 0x091B ],
    'ja'    =>  [ 0x091C ],
    'jha'   =>  [ 0x091D ],
    'nya'   =>  [ 0x091E ],
    'tta'   =>  [ 0x091F ],
    'ttha'  =>  [ 0x0920 ],
    'dda'   =>  [ 0x0921 ],
    'ddha'  =>  [ 0x0922 ],
    'nna'   =>  [ 0x0923 ],
    'ta'    =>  [ 0x0924 ],
    'tha'   =>  [ 0x0925 ],
    'da'    =>  [ 0x0926 ],
    'dha'   =>  [ 0x0927 ],
    'na'    =>  [ 0x0928 ],
    'nnna'  =>  [ 0x0929 ],
    'pa'    =>  [ 0x092A ],
    'pha'   =>  [ 0x092B ],
    'ba'    =>  [ 0x092C ],
    'bha'   =>  [ 0x092D ],
    'ma'    =>  [ 0x092E ],
    'ya'    =>  [ 0x092F ],
    'ra'    =>  [ 0x0930 ],
    'rra'   =>  [ 0x0931 ],
    'la'    =>  [ 0x0932 ],
    'lla'   =>  [ 0x0933 ],
    'llla'  =>  [ 0x0934 ],
    'va'    =>  [ 0x0935 ],
    'sha'   =>  [ 0x0936 ],
    'ssa'   =>  [ 0x0937 ],
    'sa'    =>  [ 0x0938 ],
    'ha'    =>  [ 0x0939 ],

    # alternate [ ways o ]f typing some of the stuff above
    'Ba'    =>  [ 0x092d ],  # bha
    'Ca'    =>  [ 0x091b ],  # cHa
    'Da'    =>  [ 0x0927 ],  # dHa
    'fa'    =>  [ 0x091f ],  # Ta
    'Fa'    =>  [ 0x0920 ],  # THA
    'Ga'    =>  [ 0x0918 ],  # gHa
    'Ja'    =>  [ 0x091d ],  # jHa
    'Ka'    =>  [ 0x0916 ],  # kHa
    'Ma'    =>  [ 0x0919 ],  # nG
    'Na'    =>  [ 0x0923 ],  # nna/Na
    'Pa'    =>  [ 0x092b ],  # pHa
    'Sa'    =>  [ 0x0937 ],  # SHA
    'Ta'    =>  [ 0x0925 ],  # tHa
    'Va'    =>  [ 0x0922 ],  # DHA
    'wa'    =>  [ 0x0935 ],  # wa/va
    'za'    =>  [ 0x0936 ],  # sHa
    # 'va'    =>  0x0921,  # Da  # XXX copy devrom here?

    'Wa'    =>  [ 0x0926, 0x094d, 0x0935 ],  # dwa 
    'Za'    =>  [ 0x0915, 0x094d, 0x0937 ],  # ksHa
);

my %vowels =
(
    'aa'        =>  [ 0x093E ],  # aa/A
    'A'         =>  [ 0x093E ],  # aa/A
    'i'         =>  [ 0x093F ],
    'ii'        =>  [ 0x0940 ],  # ii/I
    'I'         =>  [ 0x0940 ],  # ii/I
    'u'         =>  [ 0x0941 ],
    'uu'        =>  [ 0x0942 ],  # uu/U
    'U'         =>  [ 0x0942 ],  # uu/U
    'r'         =>  [ 0x0943 ],
    'rr'        =>  [ 0x0944 ],  # rr/R
    'R'         =>  [ 0x0944 ],  # rr/R
    'e'         =>  [ 0x0947 ],
    'ai'        =>  [ 0x0948 ],  # ai/E
    'E'         =>  [ 0x0948 ],  # ai/E
    'o'         =>  [ 0x094B ],
    'au'        =>  [ 0x094C ],
    # 'CANDRA E'  =>  0x0945,
    # 'SHORT E'   =>  0x0946,
    # 'CANDRA O'  =>  0x0949,
    # 'SHORT O'   =>  0x094A,
);

binmode (STDOUT, ":utf8");

my $max_compose_len = 6;

for my $consonant (keys %consonants)
{
    my ($i, $j);
    my @consonant_keys = split ('', $consonant);

    # virama
    # 
    printf ("  { {");
    for ($i = 0;  $i < @consonant_keys - 1;  $i++) {
        printf ("  %6s,", "GDK_$consonant_keys[$i]");
    }
    for ( ; $i < $max_compose_len;  $i++) {
        printf ("  %6s,", '0');
    }
    print qq( },  ");
    for my $character (@{$consonants{$consonant}}, 0x094d) {
        printf ("%c", $character); 
    }
    print qq(" }, /* $consonant + virama */\n);

    # consonant alone
    #
    printf ("  { {");
    for ($i = 0;  $i < @consonant_keys;  $i++) {
        printf ("  %6s,", "GDK_$consonant_keys[$i]");
    }
    for ( ; $i < $max_compose_len;  $i++) {
        printf ("  %6s,", '0');
    }
    print qq( },  ");

    for my $character (@{$consonants{$consonant}}) {
        printf ("%c", $character); 
    }
    print qq(" }, /* $consonant */\n);

    # the vowels
    #
    for my $vowel (keys %vowels)
    {
        my @vowel_keys = split ('', $vowel);

        printf ("  { {");
        for ($i = 0;  $i < @consonant_keys - 1;  $i++) {
            printf ("  %6s,", "GDK_$consonant_keys[$i]");
        }
        for ($j = 0;  $j < @vowel_keys;  $j++) {
            printf ("  %6s,", "GDK_$vowel_keys[$j]");
        }
        for ($i = $i + $j; $i < $max_compose_len;  $i++) {
            printf ("  %6s,", '0');
        }

        print qq( },  ");
        for my $character (@{$consonants{$consonant}}, @{$vowels{$vowel}}) {
            printf ("%c", $character); 
        }
        print qq(" }, /* $consonant + $vowel */\n);
    }

    print "\n";
}
