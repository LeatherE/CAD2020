Execute
./cada0094 <golden.v> <revised.v> <output>
<golden.v> and <revised.v> are input files that describe two netlists G and R in Verilog format, respectively.
<output> is the output file that shows the checking result: equivalence or non-equivalence with a witness.

Files
There will be seven files, included:
cada0094: our submission
encoder: use to encode circuid
miter: construct miter on golden and revised
abc: executable file of ABC
abc_step.txt: work on ABC
result: save the result of ABC to output
README: this readme file


