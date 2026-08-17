<project xmlns="com.autoesl.autopilot.project" name="seq2seqlite_student_proj" top="student_infer_pixel">
    <includePaths/>
    <libraryPaths/>
    <libraryFlag/>
    <Simulation>
        <SimFlow name="csim" csimMode="0" lastCsimMode="0"/>
    </Simulation>
    <files xmlns="">
        <file name="../../src/student_tb.cpp" sc="0" tb="1" cflags=" -Wno-unknown-pragmas" csimflags=" -Wno-unknown-pragmas" blackbox="false"/>
        <file name="src/student_top.cpp" sc="0" tb="false" cflags="-Isrc" csimflags="" blackbox="false"/>
        <file name="src/student_hls_types.h" sc="0" tb="false" cflags="-Isrc" csimflags="" blackbox="false"/>
        <file name="src/student_weight_convert.h" sc="0" tb="false" cflags="-Isrc" csimflags="" blackbox="false"/>
        <file name="src/student_weights_int8.h" sc="0" tb="false" cflags="-Isrc" csimflags="" blackbox="false"/>
    </files>
    <solutions xmlns="">
        <solution name="solution1" status="active"/>
    </solutions>
</project>

