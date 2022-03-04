
# ruby scripts/scaffold.rb #class-name [#class-type={*regular,menu}]

$LOAD_PATH.unshift File.dirname(__FILE__)+'/template'
$LOAD_PATH.unshift File.dirname(__FILE__)+'/ext'
$LOAD_PATH.unshift File.dirname(__FILE__)

require 'template_creator'

c_class_name = ARGV.first
c_type_name = (ARGV.size>1) ? ARGV[1] : "regular"

TemplateCreator.create(c_type_name, c_class_name)

