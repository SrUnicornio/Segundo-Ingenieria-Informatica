unico=
lista=
leelista=0

usage() { 
    echo "opciones.sh [-u arg] [-l arg ...]"
}

while [ -n "$1" ]; do

    case "$1" in
        -u )
            leelista=0;
            shift
            if [ -z "$1" ]; then
                usage
                exit 1
            fi
            unico="$1"
            ;;
        -l )
            leelista=1;
            ;;
        * )
            if [ "$leelista" -eq 1 ]; then
                lista+="$1 "
            else
                usage
                exit 1
            fi
     esac
     shift
done

if [ -n "$unico" ]; then
    echo "Unico es $unico"
fi

if [ -n "$lista" ]; then
    echo "Lista es $lista"
fi

