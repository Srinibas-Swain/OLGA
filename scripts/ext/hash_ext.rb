
class Hash
  def slice(*keys1)
    Hash[keys1.zip(self.values_at(*keys1))]
  end
end
