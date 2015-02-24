TEX=base
NAME=figure


function make_tex {
    TEX=base
    NAME=figure
    if [ -z "$1" ]; then
        NAME=$1
    fi

    echo "\\documentclass{article}"      >  ${TEX}.tex
    echo "\\usepackage{graphicx}"        >> ${TEX}.tex
    echo "\\usepackage{nopageno}"        >> ${TEX}.tex
    echo "\\usepackage[usenames]{color}" >> ${TEX}.tex
    echo "\\usepackage{underscore}"
    echo "\\begin{document}"             >> ${TEX}.tex

    for entry in ./plot_*.gp
    do
        base=${entry%.gp}
        temp_gp=${base}_temp.gp
        tex_name=${base}.tex

        echo "set terminal epslatex color" > $temp_gp
        echo "set output '${tex_name}'" >> $temp_gp
        cat $entry >> $temp_gp
        gnuplot $temp_gp
        rm $temp_gp
        echo "\\begin{center}" >> ${TEX}.tex
        echo "\\input{${tex_name}}" >> ${TEX}.tex
        echo "\\end{center}" >> ${TEX}.tex
    done

    echo "\\end{document}"           >> ${TEX}.tex
    latex -interaction=batchmode ${TEX}.tex
    dvips ${TEX}.dvi -o ${TEX}.ps
    ps2pdf14 ${TEX}.ps
    mv ${TEX}.pdf ${NAME}.pdf
    rm *.eps
    rm *.tex
    rm ${TEX}.*
}

function make_png {
    TEX=base
    NAME=figure
    if [ -z "$1" ]; then
        NAME=$1
    fi

    echo "\\documentclass{article}"      >  ${TEX}.tex
    echo "\\usepackage{graphicx}"        >> ${TEX}.tex
    echo "\\usepackage{nopageno}"        >> ${TEX}.tex
    echo "\\usepackage[usenames]{color}" >> ${TEX}.tex
    echo "\\usepackage{underscore}"
    echo "\\begin{document}"             >> ${TEX}.tex

    for entry in ./plot_*.gp
    do
        base=${entry%.gp}
        temp_gp=${base}_temp.gp
        png=${base}.png

        echo "set terminal png" > $temp_gp
        echo "set output '${png}'" >> $temp_gp
        cat $entry >> $temp_gp
        gnuplot $temp_gp
        rm $temp_gp
        echo "\\begin{figure}" >> ${TEX}.tex
        echo "\\centering" >> ${TEX}.tex
        echo "\\includegraphics[width=\textwidth]{${png}}" >> ${TEX}.tex
        echo "\\end{figure}" >> ${TEX}.tex
    done

    echo "\\end{document}"           >> ${TEX}.tex
    pdflatex   ${TEX}.tex
    mv ${TEX}.pdf ${NAME}.pdf 
    rm *.png
    rm *.tex
    rm ${TEX}.*
}
