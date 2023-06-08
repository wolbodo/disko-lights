
type Config = {
  program?: string
  brightness?: number
}

export const send = (config: Config) => {
  fetch("http://disko-grid.local/config", {
    method: "POST",
    headers:{
      'Content-Type': 'application/x-www-form-urlencoded'
    },    
    body: new URLSearchParams(Object.entries(config).map(([k, v]) => [k, v.toString()]))
  })
}