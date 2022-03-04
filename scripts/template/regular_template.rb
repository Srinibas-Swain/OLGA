load 'template.rb'

class RegularTemplate < Template
  attr_accessor :file_name
  def execute(fo_path, should_update_welcome=false)
	write_header(fo_path)
	write_src(fo_path)
	write_test()
	update_makefile()
	#update_welcome() if should_update_welcome
  end

  def write_header(fo_path='templates/regular')
	write_out(fo_path + '/header.template',
	  "include/#{file_name}.hh",
	  @params.slice('file_name', 'str_guard'))
  end

  def write_src(fo_path='templates/regular')
	params['file_name'] = file_name
	write_out(fo_path + '/src.template',
	  "src/#{file_name}.cc",
	  @params.slice('file_name'))
  end

  def write_test()
	  puts("write-test: params #{@params}")
	write_out('templates/regular/test.template',
	  "tests/#{file_name}_test.cc",
	  @params)
  end

  def update_makefile()
	  `#{GaUtils.sed} -i '/^SRC=/ s/$/ #{file_name}/' Makefile`
  end

  private
  def compute_params
	super

	object_name = file_name.split('_').map{|word| word[0]}.join()
	@params['object_name'] = object_name
  end

end


