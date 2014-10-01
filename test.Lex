#!/usr/bin/perl -w

use strict;
my $debug = 0;

my $CLASS  = "323";
my $HWK    = "4";
my $NAME   = "Lex";             # Name of program
my $TEST   = "tIJ";             # Name of test file (IJ is replaced by number)
my $ANSWER = "tIJ.t";           # Name of answer file (IJ is replaced by number)
my $DATE   = "09/25/14";        # Date script written
my $LANG   = "C";               # Language (C, Perl, ...)

my $hwkFiles = "parse.h mainBsh.c";

my $PROGRAM = "./$NAME";        # Name of executable

my %WHICH;                      # Allow individual tests on command line
@WHICH{@ARGV}++
   if (@ARGV);

$SIG{HUP} = $SIG{INT} = $SIG{QUIT}
   = $SIG{TERM} = \&cleanup;
my @UNLINK;                                     # Files to delete on signal
my $TMPDIR = "/tmp/TEST.$NAME.$$";              # Name of temporary directory
sub cleanup {
   unlink (@UNLINK);                            # Delete files
   if (-e $TMPDIR) {                            # Delete temporary directory
      system ("/bin/chmod -R +w $TMPDIR");
      system ("/bin/rm -rf $TMPDIR");
   }
   exit;
}

my $WallClock = 0;

$0 =~ s{^.*/}{};                                # Extract test type
my $TYPE = ($0 =~ m{^test\.}) ? "Public" : "Final";
print "\n$TYPE test script for $NAME ($DATE)\n\n";

&makeProgram
   unless ($LANG eq "Perl");

&limitCpuTime (2, 4);                           # Limit CPU-time per process
&limitWallClock (10);                           # Limit wall-clock per process
&limitFileSize (10000);                         # Limit size of files created
&limitHeapSize (300000);                        # Limit size of heap
&limitProcesses (1000);                         # Limit #processes

$|++;
print "\nEach test is either passed or failed; there is no partial credit.\n\n"
    . "To execute the test labelled IJ, type the command:\n"
    . "     /c/cs$CLASS/Hwk${HWK}/Tests/$TEST\n"
    . "The answer expected is in /c/cs$CLASS/Hwk${HWK}/Tests/$ANSWER.\n\n";

my $total = 0;

&header ('Basic tests of lexing');
$total += &runTest ('01', 'SIMPLE');
$total += &runTest ('02', 'REDIR_IN and REDIR_HERE');
$total += &runTest ('03', 'REDIR_OUT and REDIR_APP');
$total += &runTest ('04', 'SEP_END');
$total += &runTest ('05', 'SEP_BG and SEP_AND');
$total += &runTest ('06', 'REDIR_PIPE and SEP_OR');
$total += &runTest ('07', 'PAREN_LEFT and PAREN_RIGHT');

&header ('Stress tests of lexing');
$total += &runTest ('08', 'Long line with one SIMPLE token');
$total += &runTest ('09', 'SIMPLE token ends at non-newline whitespace');

&header ('Basic tests of history');
$total += &runTest ('10', 'history command');
$total += &runTest ('11', 'hclear command');
$total += &runTest ('12', '!!');
$total += &runTest ('13', '!N');
$total += &runTest ('14', '!?STRING? in first token');
$total += &runTest ('15', '!!*');
$total += &runTest ('16', '!!:M [M = 0, 1, ..., last]');
$total += &runTest ('17', 'Multiple substitutions in same line');

&header ('Error checking');
$total += &runTest ('18', '!! when no previous command (fail)');

&header ('All storage freed?');
$total += &runTest ('19', 'Normal');

print "\nEnd of Public Script\n";

printf ("\n%3d points Total for $NAME\n", $total);

&sectionHeader ("Possible Deductions (assessed later as appropriate)");
&possibleDeduction ( -5, "Does not make");
&possibleDeduction ( -1, "Makefile missing");
&possibleDeduction ( -1, "Makefile incorrect");
&possibleDeduction ( -1, "Log file incorrectly named");
&possibleDeduction ( -1, "Log file lacks estimated time");
&possibleDeduction ( -1, "Log file lacks total time");
&possibleDeduction ( -1, "Log file lacks statement of major difficulties");
&possibleDeduction ( -1, "Compilation errors using -Wall -std=c99 -pedantic");

if ($TYPE eq "Final") {
   print "\n";
   system ("rm -f $PROGRAM *.o")                # Cleanup if final script
      unless ($LANG eq "Perl");
}

if ($TYPE eq "Public") {                        # Reminder to students
   system ("/c/cs$CLASS/bin/checklog -noprint");
   system ("/c/cs$CLASS/bin/checkmake -noprint")
      unless ($LANG eq "Perl");
}

exit $total;


##########
# Print section header for tests
sub sectionHeader {
   printf ("\n%11s%s\n", "", @_);
}


##########
# Print header for tests
sub header {
   printf ("\n%15s%s\n", "", @_);
}

   
##########
# Print addition
sub addition {
   my ($points, $title) = @_;
   printf ("%3d point       $title\n", $points);
   return $points;
}


##########
# Print deduction
sub deduction {
   my ($points, $title) = @_;
   printf ("%3d point       $title\n", $points);
   return $points;
}


##########
# Print possible deduction
sub possibleDeduction {
   printf ("%18d %s\n", @_);
}


##########
# Run a test
sub runTest {
   my ($test, $title, $conds) = @_;
   my $where   = "/c/cs$CLASS/Hwk$HWK/Tests";   # Where to find test files
   my $run     = "/c/cs$CLASS/bin/run";         # Utility to run program
   my $diff    = "/usr/bin/diff";               # Utility to compare files
   my $head    = "/c/cs$CLASS/bin/Head";        # Utility to output files
   my $results = "/tmp/$NAME.$$";               # Results of test
   my $diffs   = "/tmp/diff.$$";                # Expected results vs. results
   my $errors  = "/tmp/errs.$$";                # Error messages generated
   my ($status, @conds, $points);

   my $testFile = "$where/$TEST";               # Name of test file
   $testFile    =~ s{IJ}{$test};
   my $answers  = "$where/$ANSWER";             # Name of answer file
   $answers     =~ s{IJ}{$test};

   return 0                                     # Either execute all tests or
      unless (keys %WHICH == 0                  #   only those on command line
	      || exists $WHICH{$test});

   (-r $testFile)
      || die ("$0: missing test file $testFile\n");

   push @UNLINK, $results, $errors, $diffs;     # Files to delete on signal

   if (-x $testFile) {
      $status = execute ($testFile, undef, $results, $errors);
   } elsif ($LANG eq "Perl") {
      $status = execute ($PROGRAM, $testFile, $results, $errors);
   } else {
      $status = execute ("$run $PROGRAM", $testFile, $results, $errors);
   }

   if (defined $conds && $conds eq "Graceful") {
      @conds = ('NORMAL', $status);
   } else {
      (-r $answers)
	 || die ("$0: missing answer file $answers\n");
      system ("$diff $answers $results  > $diffs  2>> $errors");
      system ("$head $errors $diffs");
      @conds = ('NULL', $errors,  'NULL', $diffs);
   }

   if (defined $conds && $conds eq "Error message") {
      @conds = ('NONNULL', $errors,  'NULL', $diffs);
   }

   if (defined $conds && $conds =~ m{^Deduct=(\d+)$}) {
      @conds = ('DEDUCT', $1, @conds);
   }

   $points = &correct (@conds);
   printf ("%3d point  %3s. $title\n", $points, $test);
   system ("rm -f $results $errors $diffs");
   pop @UNLINK;  pop @UNLINK;  pop @UNLINK;     # Remove added files

   return $points;
}


##########
#  correct ({[UNOP FILE] | ['NORMAL' STATUS]}*)
#
#  Return 1 if the conjunction of the specified tests is true, else 0, where:
#
#    UNOP FILE (where UNOP is either 'NULL' or 'NONNULL'):
#      Is the size of the file FILE as specified?
#
#    'NORMAL' STATUS:
#      Did the process terminate normally?
#
#    'DEDUCT' POINTS:
#      Change the point values to 0 for success, -POINTS for failure
#
sub correct {
   my $op;
   my ($success, $failure) = (1, 0);

   while ($op = shift) {
      if ($op eq 'NULL') {
	 my $arg = shift;
	 print STDERR "$op $arg\n" if $debug;
	 if (-s $arg) {
	    if ($arg =~ m{/diff\.}) {
	       print "Error: STDOUT differs from expected\n";
	    } elsif ($arg =~ m{/errs\.}) {
	       print "Error: STDERR should be empty\n";
	    } else {
	       print "Error: File $arg is nonempty\n";
	    }
	    return $failure;
	 }

      } elsif ($op eq 'NONNULL') {
	 my $arg = shift;
	 print STDERR "$op $arg\n" if $debug;
	 if (!-s $arg) {
	    if ($arg =~ m{/errs\.}) {
	       print "Error: STDERR should be nonempty\n";
	    } else {
	       print "Error: File $arg is empty\n";
	    }
	    return $failure;
	 }

      } elsif ($op eq 'NORMAL') {
	 my $arg = 127 & shift;
	 print STDERR "$op $arg\n" if $debug;
	 if ($arg != 0) {
	    print "Error: Status = $arg is nonzero\n";
	    return $failure;
	 }

      } elsif ($op eq 'DEDUCT') {
	 my $arg = shift;
	 ($success, $failure) = (0, -$arg);
      }
   }
   return $success;
}


##########
# Create program to test
sub makeProgram {
#  system ("rm -f $PROGRAM");                   # Delete program & object files
#  system ("rm -f *.o")
#     if ($TYPE eq "Final");

   (-f "Makefile" || -f "makefile")             # Give warning if no Makefile
      || warn ("$0: no makefile found\n");

   system ("/c/cs323/bin/makewarn $PROGRAM");
   ($? == 0)
      || die ("$0: cannot compile $PROGRAM\n");
}


##########
# Limit CPU-time, wall-clock-time, file-size, and/or heap-size
use BSD::Resource;

sub limitCpuTime { # (time in seconds)
   my ($soft, $hard) = @_;
   $hard = $soft
      if (! defined($hard));
   setrlimit (RLIMIT_CPU, $soft, $hard);
}

sub limitWallClock { # (time in seconds)
   my ($wall) = @_;
   $SIG{ALRM} = 'IGNORE';                       # Parent ignores alarms
   $WallClock = $wall;
}

sub limitFileSize { # (size in kilobytes)
   my ($size) = @_;
   $size *= 1024;
   setrlimit (RLIMIT_FSIZE, $size, $size);
}

sub limitHeapSize { # (size in kilobytes        # Bug: Has no effect
   my ($size) = @_;
   $size *= 1024;
   setrlimit (RLIMIT_VMEM, $size, $size);
}

sub limitProcesses { # (#processes)             # Bug: Has no effect
   my ($nproc) = @_;
   setrlimit (RLIMIT_NPROC, $nproc, $nproc);
}


##########
# Execute program after redirecting stdin, stdout, & stderr and return status
sub execute {
   my ($program, $stdin, $stdout, $stderr) = @_;
   my ($pid, $status);

   (defined ($pid = fork))                      # Create child process
      || die ("$0: fork failed\n");

   if ($pid == 0) {                             # Child process
      open (STDIN, "<$stdin")                   #  Redirect stdin
	 if (defined $stdin);
      open (STDOUT, ">$stdout")                 #  Redirect stdout
	 if (defined $stdout);
      open (STDERR, ">$stderr")                 #  Redirect stderr
	 if (defined $stderr);
      mkdir ($TMPDIR)                           #  Create a temporary directory
	 || die ("$0: mkdir $TMPDIR failed\n");
      system ("/bin/cp $PROGRAM $TMPDIR");      #    With a copy of the program
      chdir ("$TMPDIR")                         #    And cd there
	 || die ("$0: chdir $TMPDIR failed\n");
      (exec $program)                           #  Execute command
	 ||  die ("$0: exec failed\n");
   }

   alarm ($WallClock);                          # Set an alarm to interrupt in
   $SIG{ALRM} =                                 # ... $WallClock seconds
      sub {kill "TERM", $pid;
	   if (defined $stderr) {
	      open (ERROR, ">>$stderr")
		 || die ("$0: open (>>$stderr) failed\n");
	      print ERROR  "Time limit exceeded\n";
	      close (ERROR);
	   } else {
	      print STDERR "Time limit exceeded\n";
	   }
      };
   waitpid ($pid, 0);                           # Wait for child to die,
   alarm (0);                                   # ... cancel alarm,
   $status = $?;

   system ("/bin/chmod -R +w $TMPDIR");         # Delete temporary directory
   system ("/bin/rm -rf $TMPDIR");
   (! -e $TMPDIR)
      || die ("$0: cannot delete $TMPDIR\n");

   return $status;                              # ... and return exit status
}


##########
# $FILES is a list of filenames separated by commas.  Announce the
# existence of any files in the list that are local copies of files in
# /c/cs$CLASS/Hwk$HWK.  Return 1 if any do exist, else 0.
sub localCopies { # ($files)
   my ($files) = @_;
   my $deduct = 0;
   foreach my $name (split (m{,\s*}, $files)) {
      next
	 if (!-e $name);
      my $master = "/c/cs$CLASS/Hwk$HWK/$name";
      my $status = system ("/usr/bin/diff $name $master >/dev/null 2>&1") >> 8;
      if ($status == 0) {
	  $deduct = -1;
	  print "Local copy of $name exists\n"
      }
   }
   return $deduct;
}
