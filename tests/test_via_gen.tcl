set tech [::ord::get_db_tech]
puts "--- Via Generator Rules in OpenROAD Tech ---"
foreach v [$tech getVias] {
    puts "Via: [$v getName]"
}
quit
