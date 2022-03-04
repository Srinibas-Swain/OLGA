
lines = File.readlines('valgrind.out.mem')
puts("lines: #{lines.size}")

%w{perm part block}.each do |object1|
	puts "\n"*5
	puts "*"*30
	puts "for #{object1}"
	puts "*"*30

	lines_cr = lines.select {|line| line.match(/^#{object1}#/)}.sort
	lines_rm = lines.select {|line| line.match(/\~#{object1}#/)}.sort
	puts("#news: #{lines_cr.size} #deletes: #{lines_rm.size}")

	ids_cr = lines_cr.map {|line| line.split("#").last.strip.to_i }.sort 
	ids_rm = lines_rm.map {|line| line.split("#").last.strip.to_i }.sort

	puts(ids_cr.inspect)
	puts(ids_rm.inspect)
	puts "#lost: #{(ids_cr-ids_rm).size}"
	puts "first: #{(ids_cr-ids_rm)[0]}"
	puts "ten: #{(ids_cr-ids_rm)[0...10]}"
end



