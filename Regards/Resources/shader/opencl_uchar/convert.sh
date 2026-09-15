for f in *.cl; do
    LC_ALL=C tr -cd '\11\12\15\40-\176' < "$f" > "$f.tmp"
    mv "$f.tmp" "$f"
done
