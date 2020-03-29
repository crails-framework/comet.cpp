require 'pty'

module ::Guard
  class Comet < Plugin
    def initialize options = {}
      @options = options
      super
    end

    def run_on_modifications(paths)
      run_all
    end

    def compile_command
      gemspec  = Gem::Specification.find_all_by_name("comet-cpp").first
      compiler = "#{gemspec.bin_dir}/comet-make"
      command = "#{compiler}"
      command += " -c '#{@options[:cheerp_path]}'" unless @options[:cheerp_path].nil?
      command += " -i '#{@options[:cmakelists]}'"  unless @options[:cmakelists].nil?
      command += " -o '#{@options[:output]}'"      unless @options[:output].nil?
      command += " -g" if @options[:debug] == true
      command
    end

    def run_all
      PTY.spawn(compile_command) do |stdout,stdin,pid|
        begin
          stdout.each {|line| print line}
        rescue Errno::EIO
        end
        Process.wait(pid)
      end
      if $?.success? then :success else :failure end
    end
  end
end
