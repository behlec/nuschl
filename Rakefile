require "yaml"
require "open3"

class Conf
  @@mesonconf = []

  def initialize(conf)
    @conf = conf
    @good = true
    @mesonconf = []

    if @conf[:misc]
      @misc = @conf[:misc]
    elsif @conf["misc"]
      @misc = @conf["misc"]
    end

    if @conf[:run]
      @run = @conf[:run]
    end

    if @conf[:builds]
      @builds = @conf[:builds]
    elsif @conf["builds"]
      @builds = @conf["builds"]
    else
      @good = false
      $stderr.puts "Error in config, no builds defined"
      return
    end
    if !(Hash === @builds)
        @good = false
        $stderr.puts "Error in config, build parameters must be Hash"
        return
    end
    @builds.each do |k,v|
      if !((Symbol === k) || (String === k))
        @good = false
        $stderr.puts "Error in config, build name must be String or Symbol"
        return
      end
      if !(Array === v)
        @good = false
        $stderr.puts "Error in config, build parameters for #{k} must be Array"
        return
      end
      if (v.size < 1 || v.size > 3)
        @good = false
        $stderr.puts "Error in config, build parameters must be an Array of length 1 to 3"
        return
      end
      if !(String === v[0])
        @good = false
        $stderr.puts "Error in config, first build parameter for #{k} must be a String"
        return
      end
      if v.size > 1 && !(!!(v[1]) == v[1])
        @good = false
        $stderr.puts "Error in config, second build parameter for #{k} must be a Boolean"
        return
      end
      if v.size > 2 && !(Array === v[2])
        @good = false
        $stderr.puts "Error in config, third build parameter for #{k} must be an Array"
        return
      end
      if @conf[:mesonconf]
        @@mesonconf = @conf[:mesonconf]
      elsif @conf["mesonconf"]
        @@mesonconf = @conf["mesonconf"]
      end
      if !(Array === @@mesonconf)
        @good = false
        $stderr.puts "Error in config, global meson config parameters must be an Array"
        return
      end
    end
    if @conf[:default]
      @default = @conf[:default]
    elsif
      @conf["default"]
      @bdefault = @conf["default"]
    else
      @good = false
      $stderr.puts "Error in config, no default defined"
      return
    end
    if !@builds[@default]
      @good = false
      $stderr.puts "Error in config, selected default not a known build"
      return
    end
  end

  def misc
    @misc
  end

  def self.createDefault
    Conf.new(DEFAULT)
  end

  def print
    puts "Configured builds:"
    @builds.each do |k,v|
      Kernel::print "  ", k, ": ", v, "\n"
    end
    Kernel::print "default: ", @default, "\n"
    Kernel::print "mesonconf: ", @@mesonconf, "\n\n"
    Kernel::print "misc: ", CONFIG.misc, "\n"
    Kernel::print "run: ", CONFIG.run, "\n"
    puts ""
  end

  def good?; @good; end

  def builds
    @builds
  end

  def default
    @default
  end

  def run
    @run
  end

  def mesonconf
    @@mesonconf
  end

  DEFAULT = {builds: {def: ["g++", true] }, default: :def, mesonconf: ["warning_level=3"] }
end

def load_config
  if !File.exists?("Rakefile.yaml")
    puts "No config file Rakefile.yaml found, fallback to default"
    return Conf.createDefault
  end
  begin
    c = Conf.new(YAML.load_file("Rakefile.yaml"))
    if c.good?
      return c
    else
      return Conf.createDefault
    end
  rescue => e
    puts "Error loading config file Rakefile.yaml", e
  end
  return Conf.createDefault
end
CONFIG = load_config

desc "Print simple defaul yaml conf"
task "yaml" do
  puts YAML.dump(Conf::DEFAULT)
end
desc "Print the config"
task "config" do
  print "Config is ", CONFIG.good? ? "good\n" : "bad\n"
  CONFIG.print
end

class TaskBuilder
  @@confs = []
  def initialize(name, comp, test=false, mesonconf=[])
    @name = name
    @comp = comp
    @run_test = test
    @mesonconf = mesonconf

    @build_dir = "build/#{name}"

    @@confs << self
  end

  attr_reader :build_dir, :clean, :ninja, :test

  def run_tests?
    @run_test
  end

  def conf
    CONFIG.mesonconf.map { |p| "-D"+p}.join(" ") + " " + @mesonconf.map { |p| "-D"+p}.join(" ")
  end

  def create_tasks
    #create and configure build dir
    builddir = Rake::FileTask.define_task(@build_dir)
    if @comp
      builddir.enhance do
        Rake.sh "CXX=#{@comp} meson #{conf} #{@build_dir}"
      end
    else
      builddir.enhance do
        Rake.sh "meson #{conf} #{@build_dir}"
      end
    end

    # require build_dir
    foo = Rake::Task.define_task("meson#{@name}")
    foo.enhance([builddir])

    # run ninja
    ninja= Rake::Task.define_task("build#{@name}")
    ninja.enhance([foo]) do
      Rake.sh "cd #{@build_dir} && ninja"
    end
    ninja.comment = "build #{@name} with #{@comp}"
    @ninja = ninja

    # run test
    testtask= Rake::Task.define_task("test:#{@name}")
    prefix = ""
    if CONFIG.run[@name.to_sym] && CONFIG.run[@name.to_sym][:LD_LIBRARY_PATH]
      prefix = "LD_LIBRARY_PATH=#{CONFIG.run[@name.to_sym][:LD_LIBRARY_PATH]} "
    end
    testtask.enhance([ninja]) do
      Rake.sh "cd #{@build_dir} && #{prefix} ninja test"
    end
    @test=testtask

    clean = Rake::Task.define_task("clean:#{@name}")
    clean.comment = "Clean #{@build_dir}"
    clean.enhance do
      if File.exists?(@build_dir)
      Rake.sh "cd #{@build_dir} && ninja clean"
      end
    end
    @clean = clean
  end

  def self.build
    @@confs.each(&:create_tasks)
    clean = Rake::Task.define_task(:clean)
    clean.enhance(@@confs.collect(&:clean))
    clean.comment = "Clean all build dirs"
    tests = Rake::Task.define_task(:test)
    tests.enhance(@@confs.select(&:run_tests?).map(&:test))
    tests.comment = "Run selected tests"
    ninja = Rake::Task.define_task(:buildall)
    ninja.enhance(@@confs.collect(&:ninja))
    ninja.comment = "Build all build dirs"
  end
end

CONFIG.builds.each { |k,v|
  TaskBuilder.new(k, *v)
}
desc 'Default task'
task :default =>  ["test:#{CONFIG.default}"]
TaskBuilder.build

task :all =>  [:doc, :buildall, :test]

desc 'Check compatibility'
task :comp do
  if CONFIG.misc && CONFIG.misc[:libcpp]
    print "Testing std::is_invocable_v: "
    cxx = CONFIG.misc[:libcpp]
    if 0 != Open3.popen3("#{cxx} --help") do |_,_,_,thread|
        thread.value
      end
    puts "Error executing clang++"
    exit(-1)
    end
    if 0 == Open3.popen3("#{cxx} -std=c++17 -stdlib=libc++ -fsyntax-only test/compatibility/invocable.cpp") do |_,_,_,thread|
        thread.value
      end
      puts "good"
    else
      puts "bad"
    end
  else
    puts "No libc++ configured."
  end
end

desc 'Create coverage'
task :coverage => 'test:gcc' do
    Rake.sh "cd build/gcc && ninja coverage-text && cat meson-logs/coverage.txt"
end

desc 'Create documentation'
task :doc => 'doc:doxy'

desc 'Create doxygen documentation'
task 'doc:doxy' do
  sh "doxygen doc/Doxyfile"
end
