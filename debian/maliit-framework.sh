# Exports GTK_IM_MODULE and QT_IM_MODULE under Unity 8 / Lomiri

case $XDG_SESSION_DESKTOP in
  ubuntu-touch|unity8*|lomiri)
    export GTK_IM_MODULE=maliit
    export QT_IM_MODULE=maliit
    ;;
esac
